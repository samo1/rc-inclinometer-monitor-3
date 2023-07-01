from os.path import join, isfile

Import("env")

patchflag_path = ".pio/libdeps/lilygo-t-display-s3/TFT_eSPI/.patching-done"

# patch file only if we didn't do it before
if not isfile(patchflag_path):
    env.Execute("patch -p0 <tft.patch")

    def _touch(path):
        with open(path, "w") as fp:
            fp.write("")

    env.Execute(lambda *args, **kwargs: _touch(patchflag_path))
