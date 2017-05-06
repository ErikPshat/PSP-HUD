@rem
@rem perform build
@rem
@call c:\cygwin\psp-make.bat make

@rem
@rem Copy release to bin directory
@rem
@rem @if not exist bin\nul @mkdir bin
@copy hud.prx .\release\seplugins\ /y

@call "@clean.bat"
