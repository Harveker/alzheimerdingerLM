# Alzheimer's Disease Classifier with LDA (C++)

This is a **single-file C++ implementation** of a Linear Discriminant Analysis (LDA) classifier for Alzheimer's disease prediction from medical data.

## 🔒 Security & Privacy Notice

**⚠️ IMPORTANT: This repository does NOT contain any actual patient data.**

- All data files (`.csv`), model files (`.txt`), and binaries are excluded via `.gitignore`
- See `DATA_EXAMPLE.md` for data format specifications
- **Never commit real medical/patient data to version control**
- Follow HIPAA/GDPR and institutional data handling policies
- Keep all patient data on secure, encrypted local storage only

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
   - Create `alzheimer_data.csv` locally (see `DATA_EXAMPLE.md` for format)
   - Training data: 450 features + label in column 451
   - New patients: `novos_pacientes.csv` (same 450 features, no label)
   - ⚠️ **Keep data files local - they are excluded from Git**

2. **Compile**:
   ```bash
   g++ -std=c++11 -O3 -o alzheimer_classifier ProjetoLDA.cpp
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

See `DATA_EXAMPLE.md` for detailed data format specifications and security guidelines.

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
5. **All data and model files are git-ignored for security**

## 🔐 Security Best Practices

1. **Data Protection**:
   - Never commit `.csv` data files to Git
   - Store data only on encrypted local storage
   - Use anonymized patient IDs (no names, SSN, etc.)
   
2. **Model Files**:
   - Model files (`.txt`) are excluded from Git
   - They may contain patterns learned from sensitive data
   
3. **Binary Files**:
   - Compiled executables are not tracked in Git
   - Always compile from source code
   
4. **Compliance**:
   - Follow HIPAA guidelines for medical data
   - Comply with GDPR for EU data subjects
   - Adhere to institutional IRB requirements

## 🚀 Next Steps

To make this more production-ready, you could:
1. Add Makefile for easier compilation
2. Implement cross-validation
3. Add feature scaling
4. Create proper header/source file separation
