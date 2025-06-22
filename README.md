# Alzheimer's Disease Classifier with LDA (C++)

This is a **single-file C++ implementation** of a Linear Discriminant Analysis (LDA) classifier for Alzheimer's disease prediction from medical data.

## 📌 Key Features

- Pure C++ implementation (no external dependencies)
- Complete LDA algorithm from scratch:
  - Mean and covariance calculations
  - Matrix inversion
  - Regularization support
- Handles CSV data input/output
- Model persistence (save/load functionality)
- Comprehensive evaluation metrics

## 🛠️ How to Use

1. **Prepare your data**:
   - Training data: `alzheimer_data.csv` (450 features + label in column 451)
   - New patients: `novos_pacientes.csv` (same 450 features)

2. **Compile**:
   ```bash
   g++ -std=c++11 -O3 -o alzheimer_classifier main.cpp
   ```

3. **Run**:
   ```bash
   ./alzheimer_classifier
   ```

4. **Results**:
   - Model saved to: `lda_model.txt`
   - Predictions saved to: `saida.csv`

## ⚙️ Configuration

Edit these #defines at the top of the file:

```cpp
#define TRAIN_RATIO 0.8               // 80% training, 20% test
#define LAMBDA_REGULARIZATION 1e-4    // Regularization strength
#define LOAD_MODEL false              // Set true to load existing model
#define INPUT_FILE "data.csv"         // Your input file
#define OUTPUT_FILE "predictions.csv" // Output file
```

## 📊 Data Format

**Training CSV Format**:
```
ID,feature1,feature2,...,feature450,Diagnosis
1,0.12,0.34,...,0.56,P
2,0.23,0.45,...,0.67,H
```

**New Patients CSV Format**:
```
ID,feature1,feature2,...,feature450
101,0.32,0.43,...,0.65
102,0.21,0.39,...,0.72
```

## 📝 Output Example

```
ID,feature1,...,feature450,predicted_class
101,0.32,...,0.65,P
102,0.21,...,0.72,H
```

## 📊 Performance Metrics

The program outputs:
- Confusion matrix
- Accuracy
- Error rate
- Precision
- Recall
- F1-score

## 💡 Notes

1. The code handles missing values (replaces with 0.0)
2. First column is always treated as ID (skipped)
3. Diagnosis column (P/H) must be last in training data
4. Uses C++11 features

## 🚀 Next Steps

To make this more production-ready, you could:
1. Add Makefile for easier compilation
2. Implement cross-validation
3. Add feature scaling
4. Create proper header/source file separation
