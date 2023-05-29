# Features

* Synchronize different clipboards (namely _primary_ and _clipboard_).  This feature can be disabled.
* Capture clipboard history.  The length of the history is configurable.
* Select the active clipboard entry from the context menu of the status icon.
* Edit clipboard history.
* Disable history temporarily.
* Store history accross sessions.  This feature is disabled by default.

# Dependencies

* `gtkmm3`
* `boost`
* `gettext`
* `glib2`

# Packages

If you are using Archlinux you may install it from the AUR ([gclipboard-git](https://aur.archlinux.org/packages/gclipboard-git)).

# Architecture

The core of the application is made out of [composable MVC classes and templates](src/util/).  The interfaces for each functionality are defiend in the subfolders [clipboard](src/clipboard/), [preferences](src/preferences/), and [freezable](src/freezable/).  Classes that implement these are defined in the root folder.  Typically those are views that handle the interaction with GTK widgets or models that connect to the corresponding functionality with intermediate state (i.e. saving and restoring sessions via Glib or interaction with the system clipboards).
