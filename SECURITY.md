# Security Policy

## Overview

This repository contains code for analyzing medical data related to Alzheimer's disease. Due to the sensitive nature of medical data, strict security practices must be followed.

## Data Protection Requirements

### What is Protected

This repository is configured to NEVER commit:
- ✅ Patient data files (`.csv`)
- ✅ Model files (`.txt`, `.model`, `.pkl`, etc.)
- ✅ Binary executables
- ✅ IDE configuration files
- ✅ Build artifacts
- ✅ Temporary files

All these file types are excluded via `.gitignore`.

### What is Tracked

Only the following files are version controlled:
- ✅ Source code (`.cpp`, `.h`)
- ✅ Documentation (`.md`)
- ✅ Configuration metadata (build scripts, if added)

## Security Best Practices

### For Developers

1. **Before Adding Data**:
   - Always verify data is anonymized
   - Remove all personally identifiable information (PII)
   - Use random IDs instead of patient identifiers
   - Confirm compliance with institutional policies

2. **Before Committing**:
   ```bash
   # Always check what you're about to commit
   git status
   git diff --cached
   
   # Verify no sensitive files are staged
   git ls-files --cached | grep -E '\.(csv|txt|model|pkl)$'
   ```

3. **If Sensitive Data is Accidentally Committed**:
   ```bash
   # DO NOT just delete the file - it's still in git history!
   # Contact your repository administrator immediately
   # They may need to use git filter-branch or BFG Repo-Cleaner
   ```

### For Data Scientists/Researchers

1. **Data Storage**:
   - Keep all data files in a separate, secure directory outside the repo
   - Use encrypted storage for sensitive data
   - Never share data via email or unencrypted channels

2. **Data Processing**:
   - Run the code from the repository directory
   - Point the program to your secure data directory
   - Ensure output files don't contain PII

3. **Collaboration**:
   - Share only code, never data
   - Document data requirements without including real data
   - Use synthetic/mock data for examples

## Compliance

This repository should be used in compliance with:

- **HIPAA** (Health Insurance Portability and Accountability Act)
- **GDPR** (General Data Protection Regulation)
- **Institutional IRB** (Institutional Review Board) requirements
- **Local data protection regulations**

## Reporting Security Issues

If you discover a security issue:

1. **DO NOT** create a public GitHub issue
2. Contact the repository owner directly via private channels
3. Provide details about the issue
4. Wait for confirmation before discussing publicly

## Known Issues

⚠️ **Historical Data in Git History**: Prior to security improvements, some sensitive files were committed to the repository. While these files have been removed from the current version, they may still exist in the git history. To fully remove these files from history, consider:

1. Using `git filter-branch` or BFG Repo-Cleaner to remove files from history
2. Creating a fresh repository with only the secure code
3. Contacting GitHub support to clear cached copies

For now, the repository is configured to prevent any future commits of sensitive data.

## Regular Security Checks

Repository maintainers should:

- Review `.gitignore` regularly
- Audit commit history for accidental data leaks
- Update security documentation as needed
- Train team members on security practices

## Additional Resources

- [HIPAA Compliance Guidelines](https://www.hhs.gov/hipaa/index.html)
- [GDPR Information](https://gdpr.eu/)
- [GitHub Security Best Practices](https://docs.github.com/en/code-security)

---

**Remember**: When in doubt, ask before committing. It's easier to be cautious than to clean up a data leak.
