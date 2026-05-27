"""Generate C byte array headers from TTF font files."""
import os

# (filename label, C variable prefix, optional explicit source path)
fonts = [
    ("InputSans-Regular.ttf", "InputSans_Regular_ttf", None),
    ("fa-solid-900.ttf", "fa_solid_900_ttf", None),
    (
        "JetBrainsMono-Regular.ttf",
        "JetBrainsMono_Regular_ttf",
        os.path.join("..", "..", "JetBrainsMono-2.304", "fonts", "ttf", "JetBrainsMono-Regular.ttf"),
    ),
    (
        "JetBrainsMono-Bold.ttf",
        "JetBrainsMono_Bold_ttf",
        os.path.join("..", "..", "JetBrainsMono-2.304", "fonts", "ttf", "JetBrainsMono-Bold.ttf"),
    ),
    (
        "JetBrainsMono-Italic.ttf",
        "JetBrainsMono_Italic_ttf",
        os.path.join("..", "..", "JetBrainsMono-2.304", "fonts", "ttf", "JetBrainsMono-Italic.ttf"),
    ),
    (
        "JetBrainsMono-BoldItalic.ttf",
        "JetBrainsMono_BoldItalic_ttf",
        os.path.join("..", "..", "JetBrainsMono-2.304", "fonts", "ttf", "JetBrainsMono-BoldItalic.ttf"),
    ),
]

script_dir = os.path.dirname(os.path.abspath(__file__))

for filename, varname, alt_path in fonts:
    inpath = os.path.join(script_dir, alt_path) if alt_path else os.path.join(script_dir, filename)
    outpath = os.path.join(script_dir, varname + ".h")

    if not os.path.isfile(inpath):
        print("SKIP (missing): %s" % inpath)
        continue

    data = open(inpath, "rb").read()
    basename = os.path.basename(inpath)

    with open(outpath, "w") as f:
        f.write("#pragma once\n")
        f.write("// Auto-generated from %s\n" % basename)
        f.write("// Size: %d bytes\n\n" % len(data))
        f.write("static const unsigned int %s_size = %d;\n" % (varname, len(data)))
        f.write("static const unsigned char %s_data[%d] = {\n" % (varname, len(data)))
        for i in range(0, len(data), 16):
            chunk = data[i : i + 16]
            f.write("    " + ",".join("0x%02x" % b for b in chunk) + ",\n")
        f.write("};\n")

    print("%s: %d bytes -> %s" % (varname, len(data), outpath))

print("Done")
