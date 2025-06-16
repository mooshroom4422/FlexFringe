import sys
import random
from sklearn.model_selection import train_test_split

def save_data(data, filename, c):
    with open(filename, 'w') as f:
        f.write(str(len(data)) + ' ' + str(c) + '\n')
        for line in data:
            f.write(line + '\n')

def main():
    if len(sys.argv) != 5:
        print(f"usage: [input_file] [train_output] [valid_output] [test_output]")
        sys.exit(1)

    input_file = sys.argv[1]
    train_file = sys.argv[2]
    valid_file = sys.argv[3]
    test_file = sys.argv[4]

    with open(input_file, 'r') as f:
        lines = f.read().splitlines()
    n, c = map(int, lines[0].split())
    data = list(set(lines[1:]))
    x = data
    y = [line.strip().split()[0] for line in x]

    # print(x[:10])
    # print(y[:10])

    x_train, x_test, y_train, _ = train_test_split(
        x, y, test_size=0.15, stratify=y, random_state=4242
    )
    
    # print((15/85))
    x_train, x_valid, _, _ = train_test_split(
        x_train, y_train, test_size=(15/85), stratify=y_train, random_state=12321
    )

    print(len(x_train))
    print(len(x_valid))
    print(len(x_test))

    save_data(x_train, train_file, c)
    save_data(x_valid, valid_file, c)
    save_data(x_test, test_file, c)

if __name__ == "__main__":
    main()

