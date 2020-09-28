import sys
import re


def test(line):
	match = re.search(br'\("F_([A-Z0-9_]+)', line)
	field = match.group(1)

	if not match or field is None:
		return False

	field = match.group(1).lower().replace(b'_', b'')
	if b'() => %s, x => %s = x' % (field, field) in line.lower():
		return True

	field = match.group(1).lower().replace(b'_', b'.')
	return b'() => %s, x => %s = x' % (field, field) in line.lower()


def main():
	for name in sys.argv[1:]:
		file = open(name, 'rb')

		for line in file:
			if re.search(br'Add\(.*new\s+T', line) and not test(line):
				print(name, line)
		file.close()

main()
