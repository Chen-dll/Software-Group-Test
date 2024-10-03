

    #ifndef HOOKAPI_H
    #define HOOKAPI_H
     
    PROC HookAPIFunction(HMODULE hFromModule,
    PSTR pszFunctionModule,
    PSTR pszFunctionName,
    PROC pfnNewProc);
     
    #endif
