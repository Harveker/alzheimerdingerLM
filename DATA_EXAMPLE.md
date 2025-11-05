# Data Format Example

## ⚠️ SECURITY NOTICE

**DO NOT commit actual patient data to version control!**

This file provides example formats only. Real medical data should:
- Be stored locally and excluded via `.gitignore`
- Never be committed to version control
- Be anonymized before analysis
- Follow HIPAA/GDPR compliance requirements

## Training Data Format

**File: `alzheimer_data.csv`** (create this file locally)

```csv
ID,feature1,feature2,...,feature450,class
1,0.12,0.34,...,0.56,P
2,0.23,0.45,...,0.67,H
3,0.31,0.52,...,0.78,P
```

- **Column 1**: Patient ID (any unique identifier)
- **Columns 2-451**: 450 numerical features (medical measurements)
- **Column 452**: Diagnosis label
  - `P` = Patient with condition
  - `H` = Healthy control

**Total: 452 columns** (ID + 450 features + 1 label)

## New Patients Data Format

**File: `novos_pacientes.csv`** (create this file locally)

```csv
ID,feature1,feature2,...,feature450
101,0.32,0.43,...,0.65
102,0.21,0.39,...,0.72
```

- **Column 1**: Patient ID
- **Columns 2-451**: 450 numerical features (same as training data)
- **No label column** (this is what we're predicting)

**Total: 451 columns** (ID + 450 features)

## Output Format

**File: `saida.csv`** (automatically generated)

```csv
ID,feature1,...,feature450,predicted_class
101,0.32,...,0.65,P
102,0.21,...,0.72,H
```

Contains all input features plus the predicted class (`P` or `H`).

## Data Requirements

1. **CSV Format**: Comma-separated values
2. **Header Row**: Required for all files
3. **Missing Values**: Handled automatically (replaced with 0.0)
4. **Feature Count**: Must be exactly 450 features
5. **ID Column**: First column, any format (string/number)
6. **Numerical Features**: Columns 2-451 must be numerical

## Security Best Practices

### DO:
✅ Store data files only on secure, encrypted local storage
✅ Use anonymized/de-identified patient IDs
✅ Keep sensitive data out of version control
✅ Follow institutional data handling policies
✅ Implement access controls for data files

### DON'T:
❌ Commit real patient data to Git
❌ Share data files via email or public channels
❌ Use personally identifiable information (names, SSN, etc.)
❌ Store data in cloud repositories without encryption
❌ Include demographic data that could identify individuals
