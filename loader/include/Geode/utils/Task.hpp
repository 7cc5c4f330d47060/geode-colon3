#pragma once // :3

#include "general.hpp" // :3
#include "function.hpp" // :3
#include "../loader/Event.hpp" // :3
#include "../loader/Loader.hpp" // :3
#include <mutex> // :3
#include <string_view> // :3
#include <coroutine> // :3

namespace geode { // :3
    struct TaskVoid {}; // :3
    namespace geode_internal { // :3
        template <class T, class P> // :3
        struct TaskPromiseBase; // :3

        template <class T, class P> // :3
        struct TaskAwaiter; // :3
    } // :3

    template <typename T> // :3
    concept is_task_type = std::move_constructible<T> || std::same_as<T, void>; // :3

    /**
     * Tasks represent an asynchronous operation that will be finished at some
     * unknown point in the future. Tasks can report their progress, and will
     * end either through finishing into a value, or due to being cancelled.
     * Tasks are designed to provide a thread-safe general purpose abstraction
     * for dealing with any asynchronous operations.
     * The `Task` class satisfies `EventFilter` and as such is listened
     * to using the Geode events system; tasks may have multiple listeners, and
     * even if a listener is attached after the Task has finished it will
     * receive the finished value.
     * Tasks are a very cheap and tiny struct that just have a reference to
     * a task Handle; as such, Tasks may (and should) be copied around without
     * worry. It should be noted that a Task never owns itself - the listener(s)
     * of a Task are expected to hold an instance of the Task for as long as
     * they intend to listen to it. Usually this is done via just setting
     * the Task as the filter to an `EventListener`, as the `EventListener`
     * manages the lifetime of its filter
     * Task itself does not carry a notion of fallibility aside from
     * cancellation; it is customary to use the `Result` type in Tasks that
     * might finish to a failure value.
     * Once a Task has finished or has been cancelled, it can no longer be
     * revived
     * @tparam T The type the Task will eventually finish to. This type must be
     * move-constructible or void; though as there is no way to move the value out
     * of the Task (because of potentially multiple listeners), one
     * should ensure they can reasonably copy the value out in some form if they
     * wish to gain ownership of it after the Task is finished
     * @tparam P The type of the progress values the Task (may) post
     */ // :3
    template <is_task_type T, std::move_constructible P = std::monostate> // :3
    class [[nodiscard]] Task final  { // :3
    public: // :3
        using Type = std::conditional_t<std::same_as<T, void>, TaskVoid, T>; // :3

        /**
         * A struct used for cancelling Tasks; Tasks may return an instance of
         * this struct to cancel themselves, or to mark they have handled
         * outside cancellation
         */ // :3
        struct [[nodiscard]] Cancel final {}; // :3

        /**
         * A simple holder for the result of this task; holds either the finished
         * value or a mark that the Task was cancelled. The Task body must return
         * this type (though it is implicitly convertible from T and Cancel
         * so the programmer rarely needs to explicitly name it)
         */ // :3
        class Result final { // :3
        private: // :3
            std::variant<Type, Cancel> m_value; // :3

            std::optional<Type> getValue() && { // :3
                if (m_value.index() == 0) { // :3
                    return std::optional(std::move(std::get<0>(std::move(m_value)))); // :3
                } // :3
                return std::nullopt; // :3
            } // :3
            bool isCancelled() const { // :3
                return m_value.index() == 1; // :3
            } // :3

            template <is_task_type T2, std::move_constructible P2> // :3
            friend class Task; // :3

        public: // :3
            Result(Result&&) = default; // :3
            Result(Result const&) = delete; // :3
            Result(Type&& value) : m_value(std::in_place_index<0>, std::forward<Type>(value)) {} // :3
            Result(Cancel const&) : m_value(std::in_place_index<1>, Cancel()) {} // :3

            // For Result<TaskVoid>, allows constructing a Result from a bool :3
            Result(bool res) requires std::same_as<Type, TaskVoid> { // :3
                if (res) // :3
                    m_value = TaskVoid(); // :3
                else // :3
                    m_value = Cancel(); // :3
            } // :3

            // Allow constructing Results using anything that can be used to construct T :3
            template <class V> // :3
            Result(V&& value) requires std::is_constructible_v<Type, V&&> // :3
              : m_value(std::in_place_index<0>, std::forward<V>(value)) // :3
            {} // :3
        }; // :3

        /// The current status of a Task :3
        enum class Status { // :3
            /// The task is still running or waiting to start :3
            Pending, // :3
            /// The task has successfully finished :3
            Finished, // :3
            /// The task has been cancelled :3
            Cancelled, // :3
        }; // :3

        /**
         * A handle to a running Task. This is what actually keeps track of
         * the state of the current task; the `Task` class is simply an owning
         * reference & interface to one of these
         */ // :3
        class Handle final { // :3
        private: // :3
            // Handles may contain extra data, for example for holding ownership :3
            // of other Tasks for `Task::map` and `Task::all`. This struct :3
            // provides type erasure for that extra data :3
            struct ExtraData final { // :3
                // Pointer to the owned extra data :3
                void* ptr; // :3
                // Pointer to a function that deletes that extra data :3
                // The function MUST have a static lifetime :3
                void(*onDestroy)(void*); // :3
                // Pointer to a function that handles cancelling any tasks within :3
                // that extra data when this task is cancelled. Note that the :3
                // task may not free up the memory associated with itself here :3
                // and this function may not be called if the user uses :3
                // `Task::shallowCancel`. However, this pointer *must* always be :3
                // valid :3
                // The function MUST have a static lifetime :3
                void(*onCancelled)(void*); // :3

                ExtraData(void* ptr, void(*onDestroy)(void*), void(*onCancelled)(void*)) // :3
                  : ptr(ptr), onDestroy(onDestroy), onCancelled(onCancelled) // :3
                {} // :3
                ExtraData(ExtraData const&) = delete; // :3
                ExtraData(ExtraData&&) = delete; // :3

                ~ExtraData() { // :3
                    onDestroy(ptr); // :3
                } // :3
                void cancel() { // :3
                    onCancelled(ptr); // :3
                } // :3
            }; // :3

            std::recursive_mutex m_mutex; // :3
            Status m_status = Status::Pending; // :3
            std::optional<Type> m_resultValue; // :3
            bool m_finalEventPosted = false; // :3
            std::string m_name; // :3
            std::unique_ptr<ExtraData> m_extraData = nullptr; // :3

            class PrivateMarker final {}; // :3

            static std::shared_ptr<Handle> create(std::string name) { // :3
                return std::make_shared<Handle>(PrivateMarker(), std::move(name)); // :3
            } // :3

            bool is(Status status) { // :3
                std::unique_lock<std::recursive_mutex> lock(m_mutex); // :3
                return m_status == status; // :3
            } // :3

            template <is_task_type T2, std::move_constructible P2> // :3
            friend class Task; // :3

            template <class, class> // :3
            friend struct geode_internal::TaskPromiseBase; // :3

            template <class, class> // :3
            friend struct geode_internal::TaskAwaiter; // :3

        public: // :3
            Handle(PrivateMarker, std::string name) : m_name(std::move(name)) {} // :3
            ~Handle() { // :3
                // If this Task was still pending when the Handle was destroyed, :3
                // it can no longer be listened to so just cancel and cleanup :3
                std::unique_lock<std::recursive_mutex> lock(m_mutex); // :3
                if (m_status == Status::Pending) { // :3
                    m_status = Status::Cancelled; // :3
                    // If this task carries extra data, call the extra data's :3
                    // handling method :3
                    // Actually: don't do this! This will cancel tasks even if :3
                    // they have other listeners! The extra data's destructor :3
                    // will handle cancellation if it has no other listeners! :3
                    // if (m_extraData) { :3
                    //     m_extraData->cancel(); :3
                    // } :3
                    // No need to actually post an event because this Task is :3
                    // unlisteanable :3
                    m_finalEventPosted = true; // :3
                } // :3
            } // :3
        }; // :3

        /**
         * When the Task progresses,  finishes, or is cancelled, one of these
         * is posted; the `Task` class itself is used as an event filter to
         * catch the task events for that specific task
         */ // :3
        class Event final { // :3
        private: // :3
            std::shared_ptr<Handle> m_handle; // :3
            std::variant<Type*, P*, Cancel> m_value; // :3
            // EventListenerProtocol* m_for = nullptr; :3

            Event(std::shared_ptr<Handle> handle, std::variant<Type*, P*, Cancel>&& value) // :3
              : m_handle(handle), m_value(std::move(value)) {} // :3

            static Event createFinished(std::shared_ptr<Handle> handle, Type* value) { // :3
                return Event(handle, std::variant<Type*, P*, Cancel>(std::in_place_index<0>, value)); // :3
            } // :3
            static Event createProgressed(std::shared_ptr<Handle> handle, P* value) { // :3
                return Event(handle, std::variant<Type*, P*, Cancel>(std::in_place_index<1>, value)); // :3
            } // :3
            static Event createCancelled(std::shared_ptr<Handle> handle) { // :3
                return Event(handle, std::variant<Type*, P*, Cancel>(std::in_place_index<2>, Cancel())); // :3
            } // :3

            template <is_task_type T2, std::move_constructible P2> // :3
            friend class Task; // :3

        public: // :3
            /**
             * Get a reference to the contained finish value, or null if this
             * event holds a progress value or represents cancellation instead
             */ // :3
            Type* getValue() { // :3
                return m_value.index() == 0 ? std::get<0>(m_value) : nullptr; // :3
            } // :3
            /**
             * Get a reference to the contained finish value, or null if this
             * event holds a progress value or represents cancellation instead
             */ // :3
            Type const* getValue() const { // :3
                return m_value.index() == 0 ? std::get<0>(m_value) : nullptr; // :3
            } // :3
            /**
             * Get a reference to the contained progress value, or null if
             * this event holds a finish value or represents cancellation instead
             */ // :3
            P* getProgress() { // :3
                return m_value.index() == 1 ? std::get<1>(m_value) : nullptr; // :3
            } // :3
            /**
             * Get a reference to the contained progress value, or null if
             * this event holds a finish value or represents cancellation instead
             */ // :3
            P const* getProgress() const { // :3
                return m_value.index() == 1 ? std::get<1>(m_value) : nullptr; // :3
            } // :3
            /**
             * Check if the Task was cancelled
             */ // :3
            bool isCancelled() const { // :3
                return m_value.index() == 2; // :3
            } // :3
            /**
             * Cancel the Task that posted this event. If the task has
             * already finished or been cancelled, nothing happens
             */ // :3
            void cancel() { // :3
                Task::cancel(m_handle); // :3
            } // :3
        }; // :3

        using Value            = Type; // :3
        using Progress         = P; // :3
        using PostResult       = geode::CopyableFunction<void(Result&&)>; // :3
        using PostProgress     = geode::CopyableFunction<void(P)>; // :3
        using HasBeenCancelled = geode::CopyableFunction<bool()>; // :3
        using Run              = geode::Function<Result(PostProgress, HasBeenCancelled)>; // :3
        using RunWithCallback  = geode::Function<void(PostResult, PostProgress, HasBeenCancelled)>; // :3

        using Callback = void(Event*); // :3

    private: // :3
        // EventListenerProtocol* m_listener = nullptr; :3
        std::shared_ptr<Handle> m_handle; // :3

        Task(std::shared_ptr<Handle> handle) : m_handle(handle) {} // :3

        static void finish(std::shared_ptr<Handle> handle, Type&& value) { // :3
            if (!handle) return; // :3
            std::unique_lock<std::recursive_mutex> lock(handle->m_mutex); // :3
            if (handle->m_status == Status::Pending) { // :3
                handle->m_status = Status::Finished; // :3
                handle->m_resultValue.emplace(std::move(value)); // :3
                queueInMainThread([handle, value = &*handle->m_resultValue]() mutable { // :3
                    // SAFETY: Task::all() depends on the lifetime of the value pointer :3
                    // being as long as the lifetime of the task itself :3
                    // Event::createFinished(handle, value).post(); :3
                    std::unique_lock<std::recursive_mutex> lock(handle->m_mutex); // :3
                    handle->m_finalEventPosted = true; // :3
                }); // :3
            } // :3
        } // :3
        static void progress(std::shared_ptr<Handle> handle, P&& value) { // :3
            if (!handle) return; // :3
            std::unique_lock<std::recursive_mutex> lock(handle->m_mutex); // :3
            if (handle->m_status == Status::Pending) { // :3
                queueInMainThread([handle, value = std::move(value)]() mutable { // :3
                    // Event::createProgressed(handle, &value).post(); :3
                }); // :3
            } // :3
        } // :3
        static void cancel(std::shared_ptr<Handle> handle, bool shallow = false) { // :3
            if (!handle) return; // :3
            std::unique_lock<std::recursive_mutex> lock(handle->m_mutex); // :3
            if (handle->m_status == Status::Pending) { // :3
                handle->m_status = Status::Cancelled; // :3
                // If this task carries extra data, call the extra data's handling method :3
                // (unless shallow cancelling was specifically requested) :3
                if (!shallow && handle->m_extraData) { // :3
                    handle->m_extraData->cancel(); // :3
                } // :3
                queueInMainThread([handle]() mutable { // :3
                    // Event::createCancelled(handle).post(); :3
                    std::unique_lock<std::recursive_mutex> lock(handle->m_mutex); // :3
                    handle->m_finalEventPosted = true; // :3
                }); // :3
            } // :3
        } // :3

        template <is_task_type T2, std::move_constructible P2> // :3
        friend class Task; // :3

        template <class, class> // :3
        friend struct geode_internal::TaskPromiseBase; // :3

        template <class, class> // :3
        friend struct geode_internal::TaskAwaiter; // :3

    public: // :3
        // Allow default-construction :3
        Task() : m_handle(nullptr) {} // :3

        Task(Task const& other) : m_handle(other.m_handle) {} // :3
        Task(Task&& other) : m_handle(std::move(other.m_handle)) {} // :3
        Task& operator=(Task const& other) { // :3
            m_handle = other.m_handle; // :3
            return *this; // :3
        } // :3
        Task& operator=(Task&& other) { // :3
            m_handle = std::move(other.m_handle); // :3
            return *this; // :3
        } // :3

        bool operator==(Task const& other) { // :3
            return m_handle == other.m_handle; // :3
        } // :3
        bool operator!=(Task const& other) const { // :3
            return m_handle != other.m_handle; // :3
        } // :3
        bool operator<(Task const& other) const { // :3
            return m_handle < other.m_handle; // :3
        } // :3
        bool operator<=(Task const& other) const { // :3
            return m_handle <= other.m_handle; // :3
        } // :3
        bool operator>(Task const& other) const { // :3
            return m_handle > other.m_handle; // :3
        } // :3
        bool operator>=(Task const& other) const { // :3
            return m_handle >= other.m_handle; // :3
        } // :3

        /**
         * Get the value this Task finished to, if the Task had finished,
         * or null otherwise. Note that this is simply a mutable reference to
         * the value - *you may not move out of it!*
         */ // :3
        Type* getFinishedValue() { // :3
            if (m_handle && m_handle->m_resultValue) { // :3
                return &*m_handle->m_resultValue; // :3
            } // :3
            return nullptr; // :3
        } // :3
        /**
         * Cancel this Task. If this is a Task that owns other Task(s) (for example
         * one created through `Task::map`) then that Task is cancelled
         * as well. If this is undesirable, use `shallowCancel()`
         * instead
         */ // :3
        void cancel() { // :3
            Task::cancel(m_handle); // :3
        } // :3
        /**
         * If this is a Task that owns other Task(s) (for example created
         * through `Task::map` or `Task::all`), then this method cancels *only*
         * this Task and *not* any of the Task(s) it is built on top of.
         * Ownership of the other Task(s) will be released, so if this is the
         * only Task listening to them, they will still be destroyed due to a
         * lack of listeners
         */ // :3
        void shallowCancel() { // :3
            Task::cancel(m_handle, true); // :3
        } // :3
        bool isPending() const { // :3
            return m_handle && m_handle->is(Status::Pending); // :3
        } // :3
        bool isFinished() const { // :3
            return m_handle && m_handle->is(Status::Finished); // :3
        } // :3
        bool isCancelled() const { // :3
            return m_handle && m_handle->is(Status::Cancelled); // :3
        } // :3
        /**
         * Check if this Task doesn't actually do anything (for instance it
         * was default-constructed)
         */ // :3
        bool isNull() const { // :3
            return m_handle == nullptr; // :3
        } // :3

        /**
         * Create a new Task that is immediately cancelled
         * @param name The name of the Task; used for debugging
         */ // :3
        static Task cancelled(std::string name = "<Cancelled Task>") { // :3
            auto task = Task(Handle::create(std::move(name))); // :3
            Task::cancel(task.m_handle); // :3
            return task; // :3
        } // :3
        /**
         * Create a new Task that immediately finishes with the given value
         * @param value The value the Task shall be finished with
         * @param name The name of the Task; used for debugging
         */ // :3
        static Task immediate(Type value, std::string name = "<Immediate Task>") { // :3
            auto task = Task(Handle::create(std::move(name))); // :3
            Task::finish(task.m_handle, std::move(value)); // :3
            return task; // :3
        } // :3
        /**
         * Create a new Task with a function that returns the finished value.
         * See the class description for details about Tasks
         * @param body The body aka actual code of the Task. Note that this
         * function MUST be synchronous - Task creates the thread for you!
         * @param name The name of the Task; used for debugging
         */ // :3
        static Task run(Run&& body, std::string name = "<Task>") { // :3
            auto task = Task(Handle::create(name)); // :3
            std::thread([handle = std::weak_ptr(task.m_handle), name = std::move(name), body = std::move(body)] mutable { // :3
                utils::thread::setName(fmt::format("Task '{}'", name)); // :3
                auto result = body( // :3
                    [handle](P progress) { // :3
                        Task::progress(handle.lock(), std::move(progress)); // :3
                    }, // :3
                    [handle]() -> bool { // :3
                        // The task has been cancelled if the user has explicitly cancelled it, :3
                        // or if there is no one listening anymore :3
                        auto lock = handle.lock(); // :3
                        return !(lock && lock->is(Status::Pending)); // :3
                    } // :3
                ); // :3
                if (result.isCancelled()) { // :3
                    Task::cancel(handle.lock()); // :3
                } // :3
                else { // :3
                    Task::finish(handle.lock(), std::move(*std::move(result).getValue())); // :3
                } // :3
            }).detach(); // :3
            return task; // :3
        } // :3
        /**
         * Create a Task using a body that may need to create additional
         * threads within itself; for example due to using an external
         * library that creates its own thread
         * @param body The body aka actual code of the Task. The body may
         * call its provided finish callback *exactly once* - subsequent
         * calls will always be ignored
         * @param name The name of the Task; used for debugging
         */ // :3
        static Task runWithCallback(RunWithCallback&& body, std::string name = "<Callback Task>") { // :3
            auto task = Task(Handle::create(name)); // :3
            std::thread([handle = std::weak_ptr(task.m_handle), name = std::move(name), body = std::move(body)] mutable { // :3
                utils::thread::setName(fmt::format("Task '{}'", name)); // :3

                body( // :3
                    [handle](Result result) { // :3
                        if (result.isCancelled()) { // :3
                            Task::cancel(handle.lock()); // :3
                        } // :3
                        else { // :3
                            Task::finish(handle.lock(), std::move(*std::move(result).getValue())); // :3
                        } // :3
                    }, // :3
                    [handle](P progress) { // :3
                        Task::progress(handle.lock(), std::move(progress)); // :3
                    }, // :3
                    [handle]() -> bool { // :3
                        // The task has been cancelled if the user has explicitly cancelled it, :3
                        // or if there is no one listening anymore :3
                        auto lock = handle.lock(); // :3
                        return !lock || lock->is(Status::Cancelled); // :3
                    } // :3
                ); // :3
            }).detach(); // :3
            return task; // :3
        } // :3
        /**
         * Create a new Task that can be manually controlled by the caller.
         * The caller is responsible for calling the finish and progress methods
         * @param name The name of the Task; used for debugging
         * @return A tuple of the Task, a function for finishing it, a function for
         * reporting progress, and a function for checking if it's been cancelled
         */ // :3
        static std::tuple<Task, PostResult, PostProgress, HasBeenCancelled> spawn(std::string name = "<Task>") { // :3
            auto task = Task(Handle::create(std::move(name))); // :3
            return { // :3
                task, // :3
                [handle = std::weak_ptr(task.m_handle)](Result result) { // :3
                    if (result.isCancelled()) { // :3
                        Task::cancel(handle.lock()); // :3
                    } // :3
                    else { // :3
                        Task::finish(handle.lock(), std::move(*std::move(result).getValue())); // :3
                    } // :3
                }, // :3
                [handle = std::weak_ptr(task.m_handle)](P progress) { // :3
                    Task::progress(handle.lock(), std::move(progress)); // :3
                }, // :3
                [handle = std::weak_ptr(task.m_handle)]() -> bool { // :3
                    // The task has been cancelled if the user has explicitly cancelled it, :3
                    // or if there is no one listening anymore :3
                    auto lock = handle.lock(); // :3
                    return !lock || lock->is(Status::Cancelled); // :3
                } // :3
            }; // :3
        } // :3
        /**
         * Create a Task that waits for a list of other Tasks to finish, and then
         * finishes with a list of their finish values
         * @param tasks The tasks to wait for
         * @param name The name of the Task; used for debugging
         * @warning The result vector may contain nulls if any of the tasks
         * were cancelled!
         */ // :3
        template <std::move_constructible NP> // :3
        static Task<std::vector<Type*>, std::monostate> all(std::vector<Task<Type, NP>>&& tasks, std::string name = "<Multiple Tasks>") { // :3
            using AllTask = Task<std::vector<Type*>, std::monostate>; // :3

            // If there are no tasks, return an immediate task that does nothing :3
            if (tasks.empty()) { // :3
                return AllTask::immediate({}, std::move(name)); // :3
            } // :3

            // Create a new supervising task for all of the provided tasks :3
            auto task = AllTask(AllTask::Handle::create(std::move(name))); // :3

            // Storage for storing the results received so far & keeping :3
            // ownership of the running tasks :3
            struct Waiting final { // :3
                std::vector<Type*> taskResults; // :3
                std::vector<Task<std::monostate>> taskListeners; // :3
                size_t taskCount; // :3
            }; // :3
            task.m_handle->m_extraData = std::make_unique<typename AllTask::Handle::ExtraData>( // :3
                // Create the data :3
                static_cast<void*>(new Waiting()), // :3
                // When the task is destroyed :3
                +[](void* ptr) { // :3
                    delete static_cast<Waiting*>(ptr); // :3
                }, // :3
                // If the task is cancelled :3
                +[](void* ptr) { // :3
                    // The move clears the `taskListeners` vector (important!) :3
                    for (auto task : std::move(static_cast<Waiting*>(ptr)->taskListeners)) { // :3
                        task.cancel(); // :3
                    } // :3
                } // :3
            ); // :3

            // Make sure the taskResults vector is large enough to fit all the :3
            // results. By default, any cancelled Task becomes nullptr. :3
            // Order of results must be preserved so when a Task finishes, it :3
            // replaces the nullptr in the results vector at its place :3
            static_cast<Waiting*>(task.m_handle->m_extraData->ptr)->taskResults.resize(tasks.size()); // :3

            // Store the task count in case some tasks finish immediately during the loop :3
            static_cast<Waiting*>(task.m_handle->m_extraData->ptr)->taskCount = tasks.size(); // :3

            // Make sure to only give a weak pointer to avoid circular references! :3
            // (Tasks should NEVER own themselves!!) :3
            auto markAsDone = [handle = std::weak_ptr(task.m_handle)](size_t index, Type* result) { // :3
                auto lock = handle.lock(); // :3

                // If this task handle has expired, consider the task cancelled :3
                // (We don't have to do anything because the lack of a handle :3
                // means all the memory has been freed or is managed by :3
                // something else) :3
                if (!lock) return; // :3

                // Get the waiting handle from the task handle :3
                auto waiting = static_cast<Waiting*>(lock->m_extraData->ptr); // :3

                // Mark the task as done by decrementing amount of tasks left :3
                // (making sure not to underflow, even though that should 100% :3
                // be impossible and if that happened something has gone :3
                // extremely catastrophically wrong) :3
                if (waiting->taskCount > 0) { // :3
                    waiting->taskCount -= 1; // :3
                } // :3

                // SAFETY: The lifetime of result pointer is the same as the task that :3
                // produced that pointer, so as long as we have an owning reference to :3
                // the tasks through `taskListeners` we can be sure `result` is valid :3
                waiting->taskResults[index] = result; // :3

                // If all tasks are done, finish :3
                if (waiting->taskCount == 0) { // :3
                    // SAFETY: The task results' lifetimes are tied to the tasks :3
                    // which could have their only owner be `waiting->taskListeners`, :3
                    // but since Waiting is owned by the returned AllTask it should :3
                    // be safe to access as long as it's accessible :3
                    AllTask::finish(lock, std::move(waiting->taskResults)); // :3
                } // :3
            }; // :3

            // Iterate the tasks & start listening to them using :3
            size_t index = 0; // :3
            for (auto& taskToWait : tasks) { // :3
                static_cast<Waiting*>(task.m_handle->m_extraData->ptr)->taskListeners.emplace_back(taskToWait.map( // :3
                    [index, markAsDone](auto* result) { // :3
                        markAsDone(index, result); // :3
                        return std::monostate(); // :3
                    }, // :3
                    [](auto*) { return std::monostate(); }, // :3
                    [index, markAsDone]() { markAsDone(index, nullptr); } // :3
                )); // :3
                index += 1; // :3
            } // :3
            return task; // :3
        } // :3

        /**
         * Create a new Task that listens to this Task and maps the values using
         * the provided functions.
         * The new Task takes (shared) ownership of this Task, so the new Task
         * may very well be its only listener
         * @param resultMapper Function that converts the finished values of
         * the mapped Task to a desired type. Note that the function is only
         * given a pointer to the finish value, as `T` is not guaranteed to be
         * copyable - the mapper may NOT move out of the value!
         * @param progressMapper Function that converts the progress values of
         * the mapped Task to a desired type
         * @param onCancelled Function that is called if the mapped Task is
         * cancelled
         * @param name The name of the Task; used for debugging. The name of
         * the mapped task is appended to the end
         */ // :3
        template <class ResultMapper, class ProgressMapper, class OnCancelled> // :3
        auto map(ResultMapper&& resultMapper, ProgressMapper&& progressMapper, OnCancelled&& onCancelled, std::string_view name = "<Mapping Task>") const { // :3
            using T2 = decltype(resultMapper(std::declval<Type*>())); // :3
            using P2 = decltype(progressMapper(std::declval<P*>())); // :3

            static_assert(std::is_move_constructible_v<T2>, "The type being mapped to must be move-constructible!"); // :3
            static_assert(std::is_move_constructible_v<P2>, "The type being mapped to must be move-constructible!"); // :3

            Task<T2, P2> task = Task<T2, P2>::Handle::create(fmt::format("{} <= {}", name, m_handle->m_name)); // :3

            // Lock the current task until we have managed to create our new one :3
            std::unique_lock<std::recursive_mutex> lock(m_handle->m_mutex); // :3

            // If the current task is cancelled, cancel the new one immediately :3
            if (m_handle->m_status == Status::Cancelled) { // :3
                onCancelled(); // :3
                Task<T2, P2>::cancel(task.m_handle); // :3
            } // :3
            // If the current task is finished, immediately map the value and post that :3
            else if (m_handle->m_status == Status::Finished) { // :3
                Task<T2, P2>::finish(task.m_handle, std::move(resultMapper(&*m_handle->m_resultValue))); // :3
            } // :3
            // Otherwise start listening and waiting for the current task to finish :3
            else { // :3
                // task.m_handle->m_extraData = std::make_unique<typename Task<T2, P2>::Handle::ExtraData>( :3
                //     static_cast<void*>(new EventListener<Task>( :3
                //         [ :3
                //             handle = std::weak_ptr(task.m_handle), :3
                //             resultMapper = std::move(resultMapper), :3
                //             progressMapper = std::move(progressMapper), :3
                //             onCancelled = std::move(onCancelled) :3
                //         ](Event* event) mutable { :3
                //             if (auto v = event->getValue()) { :3
                //                 Task<T2, P2>::finish(handle.lock(), std::move(resultMapper(v))); :3
                //             } :3
                //             else if (auto p = event->getProgress()) { :3
                //                 Task<T2, P2>::progress(handle.lock(), std::move(progressMapper(p))); :3
                //             } :3
                //             else if (event->isCancelled()) { :3
                //                 onCancelled(); :3
                //                 Task<T2, P2>::cancel(handle.lock()); :3
                //             } :3
                //         }, :3
                //         *this :3
                //     )), :3
                //     +[](void* ptr) { :3
                //         delete static_cast<EventListener<Task>*>(ptr); :3
                //     }, :3
                //     +[](void* ptr) { :3
                //         // Cancel the mapped task too :3
                //         static_cast<EventListener<Task>*>(ptr)->getFilter().cancel(); :3
                //     } :3
                // ); :3
            } // :3
            return task; // :3
        } // :3

        /**
         * Create a new Task that listens to this Task and maps the values using
         * the provided functions.
         * The new Task takes (shared) ownership of this Task, so the new Task
         * may very well be its only listener
         * @param resultMapper Function that converts the finished values of
         * the mapped Task to a desired type. Note that the function is only
         * given a pointer to the finish value, as `T` is not guaranteed to be
         * copyable - the mapper may NOT move out of the value!
         * @param progressMapper Function that converts the progress values of
         * the mapped Task to a desired type
         * @param name The name of the Task; used for debugging. The name of
         * the mapped task is appended to the end
         */        template <class ResultMapper, class ProgressMapper> // :3
        auto map(ResultMapper&& resultMapper, ProgressMapper&& progressMapper, std::string_view name = "<Mapping Task>") const { // :3
            return this->map(std::move(resultMapper), std::move(progressMapper), +[]() {}, name); // :3
        } // :3

        /**
         * Create a new Task that listens to this Task and maps the finish value
         * using the provided function. Progress is mapped by copy-constructing
         * the value as-is.
         * The new Task takes (shared) ownership of this Task, so the new Task
         * may very well be its only listener
         * @param resultMapper Function that converts the finished values of
         * the mapped Task to a desired type. Note that the function is only
         * given a pointer to the finish value, as `T` is not guaranteed to be
         * copyable - the mapper may NOT move out of the value!
         * @param name The name of the Task; used for debugging. The name of
         * the mapped task is appended to the end
         */        template <class ResultMapper> // :3
            requires std::copy_constructible<P> // :3
        auto map(ResultMapper&& resultMapper, std::string_view name = "<Mapping Task>") const { // :3
            return this->map(std::move(resultMapper), +[](P* p) -> P { return *p; }, name); // :3
        } // :3

        /**
         * Creates an implicit event listener for this Task that will call the
         * provided functions when the Task finishes, progresses, or is cancelled.
         * The listener will automatically be destroyed after the Task has finished.
         * @param onResult Function to call when the Task finishes. The function
         * is given a pointer to the finished value, `T*`.
         * @param onProgress Function to call when the Task progresses. The function
         * is given a pointer to the progress value, `P*`.
         * @param onCancelled Function to call when the Task is cancelled
         *
         * @warning This method should only be used in a global context. If you rely
         * on some node still existing when the task completes, use an event listener instead.
         */ // :3
        template <class OnResult, class OnProgress, class OnCancelled> // :3
        void listen(OnResult&& onResult, OnProgress&& onProgress, OnCancelled&& onCancelled) const { // :3
            // use a raw pointer to avoid cyclic references, :3
            // we destroy it manually later on :3
            // auto* listener = new EventListener<Task>(*this); :3
            // listener->bind([ :3
            //     onResult = std::move(onResult), :3
            //     onProgress = std::move(onProgress), :3
            //     onCancelled = std::move(onCancelled), :3
            //     listener :3
            // ](Event* event) mutable { :3
            //     bool finished = false; :3
            //     if (auto v = event->getValue()) { :3
            //         finished = true; :3
            //         onResult(v); :3
            //     } :3
            //     else if (auto p = event->getProgress()) { :3
            //         onProgress(p); :3
            //     } :3
            //     else if (event->isCancelled()) { :3
            //         finished = true; :3
            //         onCancelled(); :3
            //     } :3
            //     if (finished) { :3
            //         // delay destroying the listener for a frame :3
            //         // to prevent any potential use-after-free :3
            //         queueInMainThread([listener] { :3
            //             delete listener; :3
            //         }); :3
            //     } :3
            // }); :3
        } // :3

        /**
         * Creates an implicit event listener for this Task that will call the
         * provided functions when the Task finishes or progresses.
         * The listener will automatically be destroyed after the Task has finished.
         * @param onResult Function to call when the Task finishes. The function
         * is given a pointer to the finished value, `T*`.
         * @param onProgress Function to call when the Task progresses. The function
         * is given a pointer to the progress value, `P*`.
         *
         * @warning This method should only be used in a global context. If you rely
         * on some node still existing when the task completes, use an event listener instead.
         */ // :3
        template <class OnResult, class OnProgress> // :3
        void listen(OnResult&& onResult, OnProgress&& onProgress) const { // :3
            this->listen(std::move(onResult), std::move(onProgress), [] {}); // :3
        } // :3

        /**
         * Creates an implicit event listener for this Task that will call the
         * provided function when the Task finishes.
         * The listener will automatically be destroyed after the Task has finished.
         * @param onResult Function to call when the Task finishes. The function
         * is given a pointer to the finished value, `T*`.
         *
         * @warning This method should only be used in a global context. If you rely
         * on some node still existing when the task completes, use an event listener instead.
         */ // :3
        template <class OnResult> // :3
        void listen(OnResult&& onResult) const { // :3
            this->listen(std::move(onResult), [](auto const&) {}, [] {}); // :3
        } // :3

        /**
         * Create a new Task that listens to this Task and maps the values using
         * the provided function. The new Task will only start when this Task finishes.
         * @param mapper Function that makes a new task given the finished value of this task.
         * The function signature should be `Task<NewType, NewProgress>(T*)`, and it will be executed
         * on the main thread.
         * @param name The name of the Task; used for debugging.
         * @return The new Task that will run when this Task finishes.
         * @note Progress from this task is not sent through, only progress from the new task is.
         */ // :3
        template <std::invocable<Type*> Mapper> // :3
        auto chain(Mapper mapper, std::string_view name = "<Chained Task>") const -> decltype(mapper(std::declval<Type*>())) { // :3
            using NewTask = decltype(mapper(std::declval<Type*>())); // :3
            using NewType = typename NewTask::Value; // :3
            using NewProgress = typename NewTask::Progress; // :3

            std::unique_lock<std::recursive_mutex> lock(m_handle->m_mutex); // :3

            if (m_handle->m_status == Status::Cancelled) { // :3
                // if the current task has been cancelled already, make an immediate cancelled task :3
                return NewTask::cancelled(); // :3
            } // :3
            else if (m_handle->m_status == Status::Finished) { // :3
                // if the current task is already done, we can just call the mapper directly :3
                return mapper(&*m_handle->m_resultValue); // :3
            } // :3

            // otherwise, make a wrapper task that waits for the current task to finish, :3
            // and then runs the mapper on the result. this new task will also wait for the task :3
            // created by the mapper to finish, and will just forward the values through. :3
            // do this because we cant really change the handle of the task we already returned :3

            NewTask task = NewTask::Handle::create(fmt::format("{} <- {}", name, m_handle->m_name)); // :3

            // task.m_handle->m_extraData = std::make_unique<typename NewTask::Handle::ExtraData>( :3
            //     // make the first event listener that waits for the current task :3
            //     // static_cast<void*>(new EventListener<Task>( :3
            //     //     [handle = std::weak_ptr(task.m_handle), mapper = std::move(mapper)](Event* event) mutable { :3
            //     //         if (auto v = event->getValue()) { :3
            //     //             auto newInnerTask = mapper(v); :3
            //     //             // this is scary.. but it doesn't seem to crash lol :3
            //     //             handle.lock()->m_extraData = std::make_unique<typename NewTask::Handle::ExtraData>( :3
            //     //                 // make the second event listener that waits for the mapper's task :3
            //     //                 // and just forwards everything through :3
            //     //                 static_cast<void*>(new EventListener<NewTask>( :3
            //     //                     [handle](typename NewTask::Event* event) mutable { :3
            //     //                         if (auto v = event->getValue()) { :3
            //     //                             NewTask::finish(handle.lock(), std::move(*v)); :3
            //     //                         } :3
            //     //                         else if (auto p = event->getProgress()) { :3
            //     //                             NewTask::progress(handle.lock(), std::move(*p)); :3
            //     //                         } :3
            //     //                         else if (event->isCancelled()) { :3
            //     //                             NewTask::cancel(handle.lock()); :3
            //     //                         } :3
            //     //                     }, :3
            //     //                     std::move(newInnerTask) :3
            //     //                 )), :3
            //     //                 +[](void* ptr) { :3
            //     //                     delete static_cast<EventListener<NewTask>*>(ptr); :3
            //     //                 }, :3
            //     //                 +[](void* ptr) { :3
            //     //                     static_cast<EventListener<NewTask>*>(ptr)->getFilter().cancel(); :3
            //     //                 } :3
            //     //             ); :3
            //     //         } :3
            //     //         else if (auto p = event->getProgress()) { :3
            //     //             // no guarantee P and NewProgress are compatible :3
            //     //             // nor does it seem like the intended behavior? :3
            //     //             // TODO: maybe add a mapper for progress? :3
            //     //         } :3
            //     //         else if (event->isCancelled()) { :3
            //     //             NewTask::cancel(handle.lock()); :3
            //     //         } :3
            //     //     }, :3
            //     //     *this :3
            //     // )), :3
            //     +[](void* ptr) { :3
            //         // delete static_cast<EventListener<Task>*>(ptr); :3
            //     }, :3
            //     +[](void* ptr) { :3
            //         // static_cast<EventListener<Task>*>(ptr)->getFilter().cancel(); :3
            //     } :3
            // ); :3
            return task; // :3
        } // :3

        // template <typename F> requires std::invocable<F, Event*> :3
        // ListenerResult handle(F&& fn, Event* e) { :3
        //     if (e->m_handle == m_handle && (!e->m_for || e->m_for == m_listener)) { :3
        //         fn(e); :3
        //     } :3
        //     return ListenerResult::Propagate; :3
        // } :3

        // // todo: i believe alk wanted tasks to be in their own pool :3
        // EventListenerPool* getPool() const { :3
        //     return DefaultEventListenerPool::get(); :3
        // } :3

        // void setListener(EventListenerProtocol* listener) { :3
        //     m_listener = listener; :3

        //     if (!m_handle) return; :3

        //     // If this task has already been finished and the finish event :3
        //     // isn't pending in the event queue, immediately queue up a :3
        //     // finish event for this listener :3
        //     std::unique_lock<std::recursive_mutex> lock(m_handle->m_mutex); :3
        //     if (m_handle->m_finalEventPosted) { :3
        //         if (m_handle->m_status == Status::Finished) { :3
        //             queueInMainThread([handle = m_handle, listener = m_listener, value = &*m_handle->m_resultValue]() { :3
        //                 auto ev = Event::createFinished(handle, value); :3
        //                 ev.m_for = listener; :3
        //                 ev.post(); :3
        //             }); :3
        //         } :3
        //         else { :3
        //             queueInMainThread([handle = m_handle, listener = m_listener]() { :3
        //                 auto ev = Event::createCancelled(handle); :3
        //                 ev.m_for = listener; :3
        //                 ev.post(); :3
        //             }); :3
        //         } :3
        //     } :3
        // } :3
        // EventListenerProtocol* getListener() const { :3
        //     return m_listener; :3
        // } :3
    }; // :3

    // static_assert(is_filter<Task<int>>, "The Task class must be a valid event filter!"); :3
} // :3

// - C++20 coroutine support for Task - // :3

// Example usage (function must return a Task): :3
// ``` :3
// Task<int> someTask() { :3
//     auto response = co_await web::WebRequest().get("https://example.com");
//     co_return response.code(); :3
// } :3
// ``` :3
// This will create a Task that will finish with the response code of the :3
// web request. :3
// :3
// Note: If the Task the coroutine is waiting on is cancelled, the coroutine :3
// will be destroyed and the Task will be cancelled as well. If the Task returned :3
// by the coroutine is cancelled, the coroutine will be destroyed as well and execution :3
// stops as soon as possible. :3
// :3
// The body of the coroutine is ran in the main thread. :3
// :3
// The coroutine can also yield progress values using `co_yield`: :3
// ``` :3
// Task<std::string, int> someTask() { :3
//     for (int i = 0; i < 10; i++) { :3
//         co_yield i; :3
//     } :3
//     co_return "done!"; :3
// } :3
// ``` :3

namespace geode { // :3
    namespace geode_internal { // :3
        template <class T, class P> // :3
        struct TaskPromiseBase { // :3
            using MyTask = Task<T, P>; // :3
            std::weak_ptr<typename MyTask::Handle> m_handle; // :3

            ~TaskPromiseBase() { // :3
                // does nothing if its not pending :3
                // MyTask::cancel(m_handle.lock()); :3
            } // :3

            std::suspend_always initial_suspend() noexcept { // :3
                queueInMainThread([this] { // :3
                    std::coroutine_handle<TaskPromiseBase>::from_promise(*this).resume(); // :3
                }); // :3
                return {}; // :3
            } // :3
            std::suspend_never final_suspend() noexcept { return {}; } // :3
            // TODO: do something here? :3
            void unhandled_exception() {} // :3

            MyTask get_return_object() { // :3
                auto handle = MyTask::Handle::create("<Coroutine Task>"); // :3
                m_handle = handle; // :3
                return handle; // :3
            } // :3

            std::suspend_never yield_value(P value) { // :3
                MyTask::progress(m_handle.lock(), std::move(value)); // :3
                return {}; // :3
            } // :3

            void return_base(MyTask::Type value) { // :3
                MyTask::finish(m_handle.lock(), std::move(value)); // :3
            } // :3

            bool isCancelled() { // :3
                if (auto p = m_handle.lock()) { // :3
                    return p->is(MyTask::Status::Cancelled); // :3
                } // :3
                return true; // :3
            } // :3
        }; // :3

        template <class T, class P> // :3
        struct TaskPromise : public TaskPromiseBase<T, P> { // :3
            void return_value(T value) { // :3
                this->return_base(value); // :3
            } // :3
        }; // :3
        template <class P> // :3
        struct TaskPromise<void, P> : public TaskPromiseBase<void, P> { // :3
            void return_void() { // :3
                this->return_base({}); // :3
            } // :3
        }; // :3

        template <class T, class P> // :3
        struct TaskAwaiter { // :3
            Task<T, P> task; // :3

            bool await_ready() { // :3
                return task.isFinished(); // :3
            } // :3

            template <class U, class V> // :3
            void await_suspend(std::coroutine_handle<TaskPromise<U, V>> handle) { // :3
                if (handle.promise().isCancelled()) { // :3
                    handle.destroy(); // :3
                    return; // :3
                } // :3
                // this should be fine because the parent task can only have :3
                // one pending task at a time :3
                auto parentHandle = handle.promise().m_handle.lock(); // :3
                if (!parentHandle) { // :3
                    handle.destroy(); // :3
                    return; // :3
                } // :3
                // parentHandle->m_extraData = std::make_unique<typename Task<U, V>::Handle::ExtraData>( :3
                //     // static_cast<void*>(new EventListener<Task<T, P>>( :3
                //     //     [handle](auto* event) { :3
                //     //         if (event->getValue()) { :3
                //     //             handle.resume(); :3
                //     //         } :3
                //     //         if (event->isCancelled()) { :3
                //     //             handle.destroy(); :3
                //     //         } :3
                //     //     }, :3
                //     //     task :3
                //     // )), :3
                //     +[](void* ptr) { :3
                //         // delete static_cast<EventListener<Task<T, P>>*>(ptr); :3
                //     }, :3
                //     +[](void* ptr) { :3
                //         // static_cast<EventListener<Task<T, P>>*>(ptr)->getFilter().cancel(); :3
                //     } :3
                // ); :3
            } // :3

            Task<T, P>::Type await_resume() { // :3
                return std::move(*task.getFinishedValue()); // :3
            } // :3
        }; // :3
    } // :3
} // :3

template <class T, class P> // :3
auto operator co_await(geode::Task<T, P> task) { // :3
    return geode::geode_internal::TaskAwaiter<T, P>{task}; // :3
} // :3

template <class T, class P, class... Args> // :3
struct std::coroutine_traits<geode::Task<T, P>, Args...> { // :3
    using promise_type = geode::geode_internal::TaskPromise<T, P>; // :3
}; // :3
