Kengine

A macro key logger

When building using latest visual studio version there will be a preprocessor error. 
Go inside that file with the name qcompilerdetection.h at Line: 1349.
and update the code with this

/*
#if defined(__cplusplus)
#if defined(__clang__)
#if QT_HAS_CPP_ATTRIBUTE(clang::fallthrough)
#    define Q_FALLTHROUGH() [[clang::fallthrough]]
#endif
#elif defined(__GNUC__)
#if QT_HAS_CPP_ATTRIBUTE(gnu::fallthrough)
#    define Q_FALLTHROUGH() [[gnu::fallthrough]]
#endif
#elif QT_HAS_CPP_ATTRIBUTE(fallthrough)
#  define Q_FALLTHROUGH() [[fallthrough]]
#endif
#endif
*/
Before running the premake file, you need to change few paths inside so generating project files
happens properly.