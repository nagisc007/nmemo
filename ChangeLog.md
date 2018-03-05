# ChangeLog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](http://keepachangelog.com/en/1.0.0/)
and this project adheres to [Semantic Versioning](http://semver.org/spec/v2.0.0.html).

## [Unreleased]

## [1.2.0] - 2018-03-05
### Added
- Tab UI.
- "Book" section in menu
- "View" section in menu
- Basic text edit menu in. (e.g. cut, copy, and paste)
- "common_types" (type defines)

### Changed
- Save data to add the save version info.
- Renamed many functions' name
  (e.g. "ToDo" is dynamic change inside,
        "Done" is purely functor.)
- Moved value defined into "common_values"
- Tab size 40px in Editor.
- Separated data and UI
- Changed template functions from utils.

### Fixed
- Remove book bug (internal data not removed).

### Removed
- Book move feature to drag and drop. (because data  and UI separated)

## [1.1.0] - 2018-01-25
### Added
- ChangeLog file.
- APP namespace in mainwindow.

### Changed
- Save method style changed using QStringList from QMap(so, old save data is not used).
- utility functor into utils.

### Fixed
- Incorrect order books when save overwrite.

## [1.0.2] - 2018-01-22
### Added
- Logo file
- Text editing mark on window title.
- PreSave feature.(Fixed lost data)

### Changed
- Signal and slots renamed.

## [1.0.1] - 2018-01-22
### Added
- Item pool features (stack).

### Changed
- Editing item changed with Book changed.

## [1.0.0] - 2018-01-16
### Added
- Main menus.
- Memo edit features.
- Save and Load features.
