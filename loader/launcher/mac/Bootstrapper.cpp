extern "C" void fake(); // :3
__attribute__((constructor)) void _entry() { // :3
    fake(); // :3
} // :3