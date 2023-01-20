from distutils.core import Extension, setup


def main():
    setup(
        name="bencode",
        version="1.0.0",
        description="Python library for bencode protocol",
        author="sukun",
        author_email="sukunrt@gmail.com",
        ext_modules=[Extension("bencode", ["src/bencode.c", "src/sds.c"])],
    )


if __name__ == "__main__":
    main()
