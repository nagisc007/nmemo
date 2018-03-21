# ChangeLog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](http://keepachangelog.com/en/1.0.0/)
and this project adheres to [Semantic Versioning](http://semver.org/spec/v2.0.0.html).

## [Unreleased]

## [1.2.1] - 2018-03-21
### Added
- Keep the page index in each books
- UI Process in mainwindow
- Ask when delete a book and a page
- Ask when unsaved file exits
- Shortcut to rename a page(old called "rename book")
- Cached the current directory path
- Tab text color change when unsaved tab(file)

### Removed
- For tab features in mainwindow
- For book features in mainwindow
- For memo features in mainwindow
- Signals in mainwindow (updated, edited, loaded and saved)
- Many functors (to convert static methods with namespaces)

### Changed
- Renamed from Tab to Book
- Renamed from Book to Page
- Renamed from Memo to Note
- Renamed from "Core" to "Core::System"
- Common values rename
- Common types rename
- Convert from functor or classes to namespaces
- Signals in mainwindow to combined "asSystemData"
- Renamed from xx book to xx item in ui file
- Shortcut to change books and pages
- Minor renamed to namespaces in utils
- Signal's name to "asXXX" from "xxed"

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
