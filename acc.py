import sys


def load_predictions(file_path):
    preds = []
    with open(file_path, 'r') as f:
        for line in f:
            clean = line.strip().split(';')[0]
            preds.append(int(clean))
    return preds

def load_labels(file_path):
    with open(file_path, 'r') as f:
        return [int(line.strip()) for line in f]

def main():
    if len(sys.argv) != 3:
        print("usage: [predictions_file] [labels_file]")
        sys.exit(1)

    pred_file = sys.argv[1]
    label_file = sys.argv[2]

    y_pred = load_predictions(pred_file)
    y_true = load_labels(label_file)

    # accuracy = sum(1 for yt, yp in zip(y_true, y_pred) if yt == yp) / len(y_true)

    tp = sum(1 for yt, yp in zip(y_true, y_pred) if yt == 1 and yp == 1) / len(y_true)
    fp = sum(1 for yt, yp in zip(y_true, y_pred) if yt == 0 and yp == 1) / len(y_true)
    tn = sum(1 for yt, yp in zip(y_true, y_pred) if yt == 0 and yp == 0) / len(y_true)
    fn = sum(1 for yt, yp in zip(y_true, y_pred) if yt == 1 and yp == 0) / len(y_true)

    se = tp/(tp+fn)
    sp = tn/(tn+fp)
    bcr = (2*se*sp)/(se+sp)

    print(f"{bcr:.4f}")

if __name__ == "__main__":
    main()

