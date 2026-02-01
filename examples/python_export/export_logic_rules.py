import argparse
import sklearn.datasets
from sklearn.tree import DecisionTreeClassifier, _tree
from sklearn.model_selection import train_test_split

def export_text_rules(tree, feature_names):
    tree_ = tree.tree_
    feature_name = [
        feature_names[i] if i != _tree.TREE_UNDEFINED else "undefined!"
        for i in tree_.feature
    ]

    rules = []

    def recurse(node, current_rule):
        if tree_.feature[node] != _tree.TREE_UNDEFINED:
            name = feature_name[node]
            threshold = tree_.threshold[node]

            # Left child (<= threshold)
            recurse(tree_.children_left[node], current_rule + [f"({name} <= {threshold:.2f})"])
            # Right child (> threshold)
            recurse(tree_.children_right[node], current_rule + [f"({name} > {threshold:.2f})"])
        else:
            # Leaf node
            class_id = tree_.value[node].argmax()
            rule_str = " & ".join(current_rule)
            rules.append(f"IF {rule_str} THEN Class={class_id}")

    recurse(0, [])
    return rules

def main():
    parser = argparse.ArgumentParser(description="Export Scikit-Learn Decision Tree to Logic Rules")
    parser.add_argument("--output", type=str, default="rules.txt", help="Output file path")
    args = parser.parse_args()

    # 1. Train a simple model (Iris dataset)
    print("Training Decision Tree on Iris dataset...")
    iris = sklearn.datasets.load_iris()
    X_train, X_test, y_train, y_test = train_test_split(iris.data, iris.target, random_state=42)

    clf = DecisionTreeClassifier(max_depth=3)
    clf.fit(X_train, y_train)

    accuracy = clf.score(X_test, y_test)
    print(f"Model Accuracy: {accuracy:.2f}")

    # 2. Convert to Rules
    print("Converting to Logic Rules...")
    rules = export_text_rules(clf, iris.feature_names)

    # 3. Save
    with open(args.output, "w") as f:
        for r in rules:
            f.write(r + "\n")
            print(r)

    print(f"Rules saved to {args.output}")

if __name__ == "__main__":
    main()
