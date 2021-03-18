on count_windows_on_current_space(process_name)
	tell application "System Events"
		tell process process_name
			return count of windows
		end tell
	end tell
end count_windows_on_current_space

if application "iTerm" is running then
	tell application "System Events" to tell process "iTerm2"
		if my count_windows_on_current_space("iTerm2") = 0 then
			-- click menu item "New Window" of menu "Shell" of menu bar 1
			tell application "iTerm"
				create window with default profile
			end tell
			set frontmost to true
		else
			set frontmost to true
		end if
	end tell

	(*
	tell application "System Events"
		set frontApp to first application process whose frontmost is true
		set frontAppName to name of frontApp
	end tell

	if frontAppName is not "iTerm2" then
		frontAppName
	end if
	*)
else
	activate application "iTerm"
end if