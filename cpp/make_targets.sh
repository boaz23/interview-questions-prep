#!/bin/bash

set -e

print_phony_targets() {
	local -a phony_targets
	phony_targets=(
		$(
			make -npq "$@" |\
			sed -nE 's/^\.PHONY:(.*)$/\1/p' |\
			tail -1
		)
	)

	echo "${phony_targets[@]}"

	# local IFS=$' \t\n' script=$(_make_target_script)
	# printf "%s\n" "${phony_targets[@]}" | command sed -e "$script"
}

print_phony_targets "$@"
