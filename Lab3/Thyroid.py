from matplotlib import pyplot as plt
import pandas as pd
from sklearn import tree
from sklearn.tree import DecisionTreeClassifier
from sklearn.model_selection import train_test_split
from sklearn import metrics
from sklearn.preprocessing import LabelEncoder

# Load dataset
col_names = ['Age', 'Gender', 'Smoking', 'Hx Smoking', 'Hx Radiothreapy',
             'Thyroid Function', 'Physical Examination', 'Adenopathy', 'Pathology', 'Focality',
             'Risk', 'T', 'N', 'M', 'Stage', 'Response', 'Recurred']
pima = pd.read_csv("Thyroid_Diff.csv", header=None, names=col_names, skiprows=1)

gender_mapping = {'M' : 1 , 'F': 2}
smoking_mapping = {'No': 1, 'Yes' : 2}
Hx_Smoking_mapping = {'No' : 1, 'Yes' : 2}
Hx_Radiothreapy_mapping = {'No' : 1, 'Yes' : 2}
Thyroid_Function_mapping = {'Clinical Hyperthyroidism' : 1, 'Clinical Hypothyroidism' : 2, 'Euthyroid' : 3, 'Subclinical Hyperthyroidism' : 4, 'Subclinical Hypothyroidism' : 5}
Physical_Examination_mapping = {'Diffuse goiter' : 1, 'Multinodular goiter' : 2, 'Normal' : 3, 'Single nodular goiter-left': 4, 'Single nodular goiter-right' : 5}
Adenopathy_mapping = {'Bilateral' : 1, 'Extensive' : 2, 'Left' : 3, 'No' : 4, 'Posterior' : 5, 'right' : 6}
Pathology_mapping = {'Follicular' : 1, 'Hurthel cell' : 2, 'Micropapillary' : 3, 'Papillary' : 4} 
Focality_mapping = {'Multi-Focal' : 1, 'Uni-Focal' : 2}
risk_mapping = {'Low': 1, 'Intermediate': 2, 'High': 3} 
T_mapping = {'T1a' : 1, 'T1b' : 2, 'T2': 3, 'T3a' : 4, 'T3b' : 5, 'T4a' : 6, 'T4b' : 7}
N_mapping = {'N0' : 1, 'N1a': 2, 'N1b' : 3}
M_mapping = {'M0' : 1, 'M1' : 2}
stage_mapping = {'I': 1, 'II': 2, 'III': 3, 'IVA': 4, 'IVB' : 5}
response_mapping ={'Biochemical Incomplete' : 1, 'Excellent' : 2, 'Indeterminate' : 3, 'Structural Incomplete' : 4}
Recurred_mapping = {'No' : 1, 'Yes' : 2}

pima['Gender'] = pima['Gender'].map(gender_mapping)
pima['Smoking'] = pima['Smoking'].map(smoking_mapping)
pima['Hx Smoking'] = pima['Hx Smoking'].map(Hx_Smoking_mapping)
pima['Hx Radiothreapy'] = pima['Hx Radiothreapy'].map(Hx_Radiothreapy_mapping)
pima['Thyroid Function'] = pima['Thyroid Function'].map(Thyroid_Function_mapping)
pima['Physical Examination'] = pima['Physical Examination'].map(Physical_Examination_mapping)
pima['Adenopathy'] = pima['Adenopathy'].map(Adenopathy_mapping)
pima['Pathology'] = pima['Pathology'].map(Pathology_mapping)
pima['Focality'] = pima['Focality'].map(Focality_mapping)
pima['Risk'] = pima['Risk'].map(risk_mapping)
pima['T'] = pima['T'].map(T_mapping)
pima['N'] = pima['N'].map(N_mapping)
pima['M'] = pima['M'].map(M_mapping)
pima['Stage'] = pima['Stage'].map(stage_mapping)
pima['Response'] = pima['Response'].map(response_mapping)
pima['Recurred'] = pima['Recurred'].map(Recurred_mapping)

# Define features and target variable
feature_cols = [col for col in pima.columns if col != 'Recurred']  # All columns except 'Recurred'
X = pima[feature_cols]  # Features
y = pima['Recurred']  # Target variable

# Split dataset into training set and test set
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.3, random_state=1)

# Create Decision Tree Classifier
clf = DecisionTreeClassifier(criterion='entropy')

####      SAME RESULT     ####
clf = DecisionTreeClassifier(criterion='entropy', random_state=1)
##############################

# Train Decision Tree Classifier
clf = clf.fit(X_train, y_train)

# Predict the response for test dataset
y_pred = clf.predict(X_test)

# Model Accuracy
print("Accuracy:", metrics.accuracy_score(y_test, y_pred))

# Ploting the tree
plt.figure(figsize=(15,8))
tree.plot_tree(clf, filled=True, rounded=True, max_depth=3, fontsize=10,
               feature_names=feature_cols, class_names=['No', 'Yes'])
plt.title("Decision tree trained on thyroid dataset")
plt.show()
