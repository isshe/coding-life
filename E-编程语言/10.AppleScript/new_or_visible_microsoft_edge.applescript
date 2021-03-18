on count_windows_on_current_space(process_name)
	tell application "System Events"
		tell process process_name
			return count of windows
		end tell
	end tell
end count_windows_on_current_space

on new_or_visible_window(process_name)
	if application process_name is running then
		tell application "System Events" to tell process process_name
			if my count_windows_on_current_space(process_name) = 0 then
				-- activate application "Microsoft Edge"
				-- click menu item "???" of menu "Shell" of menu bar 1
				(*
				tell application "iTerm"
					create window with default profile
				end tell
				*)
				tell application "Microsoft Edge"
					open location "https://www.google.com.hk"
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

		if frontAppName is not process_name then
			frontAppName
		end if
        *)

	else
		activate application "Microsoft Edge"
	end if


end new_or_visible_window

new_or_visible_window("Microsoft Edge")
