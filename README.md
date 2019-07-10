# ext
Print file (s) extension (s).

Inspired by [what-to-code](https://github.com/joereynolds/what-to-code)

## Requirements
- build-essentials
- cmake

## Installation
```bash
$ git clone https://github.com/Tarik02/ext
$ cd ext
$ mkdir build
$ cd build
$ cmake ..
$ cmake --build .
$ cmake --build . --target install
```

## Usage:
```
Usage:
	ext [options] [--] [filenames...]  read filenames from arguments
	ext [options] -                    read filenames from stdin

Options:
	-h       --help                    display this and exit
	-V       --version                 display version info
	-d       --dot                     print dot before every extension
	-l [SEP] --line=[SEP]              print output in single line with specified separator [default: ' ']
	-s [SEP] --separator=[SEP]         input separator (only if reading from stdin) [default: ' ']
```
