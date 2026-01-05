# ScriptSource-Dumper

A fast, aesthetic C++ tool designed to extract `scriptsrc.clump` archives from **Call of Duty: Black Ops II (T6)**.

**Created by Lil Poop**

![Banner](https://img.shields.io/badge/Made%20with-Love-ff9a9e?style=for-the-badge) ![Lang](https://img.shields.io/badge/Lang-C%2B%2B-ff69b4?style=for-the-badge&logo=c%2B%2B&logoColor=white) ![OS](https://img.shields.io/badge/OS-Windows-fbc2eb?style=for-the-badge&logo=windows&logoColor=white) ![Status](https://img.shields.io/badge/Status-Cute-fad0c4?style=for-the-badge) ![Sparkles](https://img.shields.io/badge/Sparkles-%E2%9C%A8-fd79a8?style=for-the-badge)

<p align="center">
  <img src="https://i.imgur.com/DKt1nti.gif" width="400">
</p>

## ✨ About

- **📦 scriptsrc.clump**: A custom archive format used in **Call of Duty: Black Ops II** to store game scripts for debugging purposes.
- **🛠️ The Tool**: A specialized extractor that parses the archive's Table of Contents (TOC) and data chunks.
- **📝 Name Recovery**: Automatically locates the internal name table to restore original filenames and directory structures.
- **🔧 1:1 Accuracy**: Extraction logic derived directly from IDA pseudocode to ensure perfect output.

## 🛠️ Building

### Prerequisites
- Visual Studio 2022+
- MSVC v145 Toolset

### Build Instructions

1. Open `ScriptSource-Dumper.sln` in Visual Studio 2022+.
2. Build the solution

Or via command line:
```powershell
msbuild ScriptSource-Dumper.sln /p:Configuration=Release /p:Platform=x64
```

## 📖 Usage

Run the tool from the command line or drag and drop a `.clump` file onto the executable.

```bash
ScriptSource-Dumper.exe <path_to_scriptsrc.clump> [output_directory]
```

- **path_to_scriptsrc.clump**: Path to the target clump file.
- **output_directory** *(Optional)*: Destination folder. Defaults to `extracted/`.

## ⚠️ Disclaimer

This tool is for educational and research purposes only. 
