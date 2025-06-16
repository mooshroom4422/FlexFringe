import sys
from sklearn.metrics import precision_score, recall_score, f1_score, accuracy_score, confusion_matrix

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
    if len(sys.argv) != 4:
        print("usage: [predictions_file] [labels_file] [out_file]")
        sys.exit(1)

    pred_file = sys.argv[1]
    label_file = sys.argv[2]
    output_file = sys.argv[3]

    y_pred = load_predictions(pred_file)
    y_true = load_labels(label_file)

    tn, fp, fn, tp = confusion_matrix(y_true, y_pred).ravel()
    se = tp/(tp+fn)
    sp = tn/(tn+fp)
    bcr = (2*se*sp)/(se+sp)

    precision = precision_score(y_true, y_pred, zero_division=0)
    recall = recall_score(y_true, y_pred, zero_division=0)
    f1 = f1_score(y_true, y_pred, zero_division=0)
    accuracy = accuracy_score(y_true, y_pred)

    print(y_true)
    print(y_pred)

    with open(output_file, 'w') as out:
        out.write(f"precision={precision:.4f}\n")
        out.write(f"recall={recall:.4f}\n")
        out.write(f"f1={f1:.4f}\n")
        out.write(f"bcr={bcr:.4f}\n")
        out.write(f"tp={tp:.4f}\n")
        out.write(f"tn={tn:.4f}\n")
        out.write(f"fp={fp:.4f}\n")
        out.write(f"fn={fn:.4f}\n")
        out.write(f"accuracy={accuracy:.4f}\n")

if __name__ == "__main__":
    main()

