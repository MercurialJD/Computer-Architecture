import os
import json


def check_assembly():
    LoC = 0
    comments = 0
    folder_found = False
    for (root, dirs, files) in os.walk('./src/assembly'):
        if root == "./src/assembly":
            folder_found = True
            if len(dirs) > 0:
                return (0, "Do not create subdirectories!")
            if len(files) < 3:
                return (0, "Not enough assembly functions!")

            for file in files:
                with open("./src/assembly/" + file, encoding='utf-8') as f:
                    for line in f.readlines():
                        text = line.strip()
                        if "/*" in text or "//" in text:
                            return (0, "Do not use /* ... */ or // in assembly comments!")
                        elif text.startswith('#'):
                            comments += 1
                        elif '#' in text:
                            LoC += 1
                            comments += 1
                        elif text != '':
                            LoC += 1

    if not folder_found:
        return (0, "./src/assembly not found!")

    if LoC * 0.4 > comments:
        return (0, "Lines of code = {0}, Lines of comments = {1}, not enough comments.".format(LoC, comments))
    else:
        return (1, "Lines of code = {0}, Lines of comments = {1}, passed.".format(LoC, comments))


if __name__ == '__main__':
    result, message = check_assembly()
    print(json.dumps({"scores": {"result": result, "message": message}}))