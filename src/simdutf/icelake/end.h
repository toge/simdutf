SIMDUTF_UNTARGET_REGION
#undef SIMDUTF_IMPLEMENTATION

#if SIMDUTF_GCC11ORMORE // workaround for https://gcc.gnu.org/bugzilla/show_bug.cgi?id=105593
SIMDUTF_POP_DISABLE_WARNINGS
#endif // end of workaround