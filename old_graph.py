import os
import sys
import matplotlib.pyplot as plt

args = sys.argv
dir = args[1]
results = {}
ile = {}

ens = []
pref = []
suf = []

for dirpath, dirnames, filenames in os.walk(dir):
    if 'res.txt' in filenames:
        with open(os.path.join(dirpath, 'res.txt'), 'r') as file:
            tmp = []
            norm_acc = 0
            print(dirpath)
            for line in file:
                accuracy_str, name = line.strip().split(';')
                accuracy = float(accuracy_str)
                tmp.append((name, accuracy))
                if name == "tmp/model_00.dat.ff.final.json.res":
                    norm_acc = accuracy
                    pref.append(accuracy)
                    print(name, accuracy)
                    print(accuracy_str, name)
                if name == "tmp/final.res":
                    ens.append(accuracy)
                # if name == "tmp/model_12.dat.ff.final.json.res":
                #     suf.append(accuracy)
            if not ile:
                for (name, _) in tmp:
                    results[name] = 0
                    if name == "tmp/model_00.dat.ff.final.json.res":
                        continue
                    ile[name] = 0
            for (name, acc) in tmp:
                results[name] += acc/100
                if acc > norm_acc:
                    print("better:", name, acc, norm_acc)
                    ile[name] += 1

print(results)

x = list(results.keys())
y = list(results.values())

print(x)
print(y)

plt.figure(figsize=(10, 6))
plt.bar(x, y)
plt.xlabel('Accuracy')
plt.title('Model Accuracies')
plt.ylim(0.6, 0.9)
plt.xticks(rotation=45)
plt.tight_layout()
plt.show()

plt.bar(ile.keys(), ile.values())
plt.xlabel('Models')
plt.ylabel('Times better than native')
plt.title('Number of times each method was better than the native one')
plt.xticks(rotation=45)
plt.tight_layout()
plt.show()

indices = list(range(len(ens)))
plt.plot(indices, ens, color='orange', marker='o', label='ensemble')
plt.plot(indices, pref, color='red', marker='o', label='prefix')
# plt.plot(indices, suf, color='green', marker='o', label='sufix')
plt.legend()
plt.grid()
plt.show()

print(pref)
