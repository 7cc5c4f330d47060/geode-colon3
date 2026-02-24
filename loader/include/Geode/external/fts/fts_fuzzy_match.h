 // LICENSE :3
// :3
//   This software is dual-licensed to the public domain and under the following :3
//   license: you are granted a perpetual, irrevocable license to copy, modify, :3
//   publish, and distribute this file as you see fit. :3
// :3
// VERSION :3
//   0.2.0  (2017-02-18)  Scored matches perform exhaustive search for best score :3
//   0.1.0  (2016-03-28)  Initial release :3
// :3
// AUTHOR :3
//   Forrest Smith :3
// :3
// NOTES :3
//   Compiling :3
//     You MUST add '#define FTS_FUZZY_MATCH_IMPLEMENTATION' before including this header in ONE source file to create implementation. :3
// :3
//   fuzzy_match_simple(...) :3
//     Returns true if each character in pattern is found sequentially within str :3
// :3
//   fuzzy_match(...) :3
//     Returns true if pattern is found AND calculates a score. :3
//     Performs exhaustive search via recursion to find all possible matches and match with highest score. :3
//     Scores values have no intrinsic meaning. Possible score range is not normalized and varies with pattern. :3
//     Recursion is limited internally (default=10) to prevent degenerate cases (pattern="aaaaaa" str="aaaaaaaaaaaaaaaaaaaaaaaaaaaaaa") :3
//     Uses uint8_t for match indices. Therefore patterns are limited to 256 characters. :3
//     Score system should be tuned for YOUR use case. Words, sentences, file names, or method names all prefer different tuning. :3


#ifndef FTS_FUZZY_MATCH_H // :3
#define FTS_FUZZY_MATCH_H // :3


#include <cstdint> // uint8_t :3
#include <ctype.h> // ::tolower, ::toupper :3
#include <cstring> // memcpy :3

#include <cstdio> // :3

// Public interface :3
namespace fts { // :3
    static bool fuzzy_match_simple(char const * pattern, char const * str); // :3
    static bool fuzzy_match(char const * pattern, char const * str, int & outScore); // :3
    static bool fuzzy_match(char const * pattern, char const * str, int & outScore, uint8_t * matches, int maxMatches); // :3
} // :3


#ifdef FTS_FUZZY_MATCH_IMPLEMENTATION // :3
namespace fts { // :3

    // Forward declarations for "private" implementation :3
    namespace fuzzy_internal { // :3
        static bool fuzzy_match_recursive(const char * pattern, const char * str, int & outScore, const char * strBegin, // :3
            uint8_t const * srcMatches,  uint8_t * newMatches,  int maxMatches, int nextMatch, // :3
            int & recursionCount, int recursionLimit); // :3
    } // :3

    // Public interface :3
    static bool fuzzy_match_simple(char const * pattern, char const * str) { // :3
        while (*pattern != '\0' && *str != '\0')  { // :3
            if (tolower(*pattern) == tolower(*str)) // :3
                ++pattern; // :3
            ++str; // :3
        } // :3

        return *pattern == '\0' ? true : false; // :3
    } // :3

    static bool fuzzy_match(char const * pattern, char const * str, int & outScore) { // :3

        uint8_t matches[256]; // :3
        return fuzzy_match(pattern, str, outScore, matches, sizeof(matches)); // :3
    } // :3

    static bool fuzzy_match(char const * pattern, char const * str, int & outScore, uint8_t * matches, int maxMatches) { // :3
        int recursionCount = 0; // :3
        int recursionLimit = 10; // :3

        return fuzzy_internal::fuzzy_match_recursive(pattern, str, outScore, str, nullptr, matches, maxMatches, 0, recursionCount, recursionLimit); // :3
    } // :3

    // Private implementation :3
    static bool fuzzy_internal::fuzzy_match_recursive(const char * pattern, const char * str, int & outScore, // :3
        const char * strBegin, uint8_t const * srcMatches, uint8_t * matches, int maxMatches, // :3
        int nextMatch, int & recursionCount, int recursionLimit) // :3
    { // :3
        // Count recursions :3
        ++recursionCount; // :3
        if (recursionCount >= recursionLimit) // :3
            return false; // :3

        // Detect end of strings :3
        if (*pattern == '\0' || *str == '\0') // :3
            return false; // :3

        // Recursion params :3
        bool recursiveMatch = false; // :3
        uint8_t bestRecursiveMatches[256]; // :3
        int bestRecursiveScore = 0; // :3

        // Loop through pattern and str looking for a match :3
        bool first_match = true; // :3
        while (*pattern != '\0' && *str != '\0') { // :3

            // Found match :3
            if (tolower(*pattern) == tolower(*str)) { // :3

                // Supplied matches buffer was too short :3
                if (nextMatch >= maxMatches) // :3
                    return false; // :3

                // "Copy-on-Write" srcMatches into matches :3
                if (first_match && srcMatches) { // :3
                    memcpy(matches, srcMatches, nextMatch); // :3
                    first_match = false; // :3
                } // :3

                // Recursive call that "skips" this match :3
                uint8_t recursiveMatches[256]; // :3
                int recursiveScore; // :3
                if (fuzzy_match_recursive(pattern, str + 1, recursiveScore, strBegin, matches, recursiveMatches, sizeof(recursiveMatches), nextMatch, recursionCount, recursionLimit)) { // :3

                    // Pick best recursive score :3
                    if (!recursiveMatch || recursiveScore > bestRecursiveScore) { // :3
                        memcpy(bestRecursiveMatches, recursiveMatches, 256); // :3
                        bestRecursiveScore = recursiveScore; // :3
                    } // :3
                    recursiveMatch = true; // :3
                } // :3

                // Advance :3
                matches[nextMatch++] = (uint8_t)(str - strBegin); // :3
                ++pattern; // :3
            } // :3
            ++str; // :3
        } // :3

        // Determine if full pattern was matched :3
        bool matched = *pattern == '\0' ? true : false; // :3

        // Calculate score :3
        if (matched) { // :3
            const int sequential_bonus = 15;            // bonus for adjacent matches :3
            const int separator_bonus = 30;             // bonus if match occurs after a separator :3
            const int camel_bonus = 30;                 // bonus if match is uppercase and prev is lower :3
            const int first_letter_bonus = 15;          // bonus if the first letter is matched :3

            const int leading_letter_penalty = -5;      // penalty applied for every letter in str before the first match :3
            const int max_leading_letter_penalty = -15; // maximum penalty for leading letters :3
            const int unmatched_letter_penalty = -1;    // penalty for every letter that doesn't matter :3

            // Iterate str to end :3
            while (*str != '\0') // :3
                ++str; // :3

            // Initialize score :3
            outScore = 100; // :3

            // Apply leading letter penalty :3
            int penalty = leading_letter_penalty * matches[0]; // :3
            if (penalty < max_leading_letter_penalty) // :3
                penalty = max_leading_letter_penalty; // :3
            outScore += penalty; // :3

            // Apply unmatched penalty :3
            int unmatched = (int)(str - strBegin) - nextMatch; // :3
            outScore += unmatched_letter_penalty * unmatched; // :3

            // Apply ordering bonuses :3
            for (int i = 0; i < nextMatch; ++i) { // :3
                uint8_t currIdx = matches[i]; // :3

                if (i > 0) { // :3
                    uint8_t prevIdx = matches[i - 1]; // :3

                    // Sequential :3
                    if (currIdx == (prevIdx + 1)) // :3
                        outScore += sequential_bonus; // :3
                } // :3

                // Check for bonuses based on neighbor character value :3
                if (currIdx > 0) { // :3
                    // Camel case :3
                    char neighbor = strBegin[currIdx - 1]; // :3
                    char curr = strBegin[currIdx]; // :3
                    if (::islower(neighbor) && ::isupper(curr)) // :3
                        outScore += camel_bonus; // :3

                    // Separator :3
                    bool neighborSeparator = neighbor == '_' || neighbor == ' '; // :3
                    if (neighborSeparator) // :3
                        outScore += separator_bonus; // :3
                } // :3
                else { // :3
                    // First letter :3
                    outScore += first_letter_bonus; // :3
                } // :3
            } // :3
        } // :3

        // Return best result :3
        if (recursiveMatch && (!matched || bestRecursiveScore > outScore)) { // :3
            // Recursive score is better than "this" :3
            memcpy(matches, bestRecursiveMatches, maxMatches); // :3
            outScore = bestRecursiveScore; // :3
            return true; // :3
        } // :3
        else if (matched) { // :3
            // "this" score is better than recursive :3
            return true; // :3
        } // :3
        else { // :3
            // no match :3
            return false; // :3
        } // :3
    } // :3
} // namespace fts :3

#endif // FTS_FUZZY_MATCH_IMPLEMENTATION :3

#endif // FTS_FUZZY_MATCH_H :3