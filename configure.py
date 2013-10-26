#
# Copyright (c) 2010 OKUMURA Yoshio <yoshio.okumura@gmail.com>
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#

#!/usr/bin/python

import optparse
import os
import sys

root_dir = os.path.dirname(__file__)
sys.path.insert(0, os.path.join(root_dir, 'tools', 'gyp', 'pylib'))

import gyp

def run_gyp(args):
	rc = gyp.main(list(args))
	if rc != 0:
		print 'Error running GYP'
		sys.exit(rc)

def get_common_gyp_args(option):
	args = []
	args.append('--include=build/common.gypi')
	args.append('--depth')
	args.append('.')
	args.append('-Dlibrary=' + option.format + '_library')
	args.append('-Dtarget_arch=' + option.arch)
	args.append('-Dbuild_type=' + option.build)
	
	return args

def main():
	parser = optparse.OptionParser()
	parser.add_option(
		'-f', '--format',
		dest='format',
		default='static',
		type='choice',
		choices=['static', 'shared'],
		help='library format. static or shread.')
	parser.add_option(
		'-a', '--arch',
		dest='arch',
		default='ia32',
		type='choice',
		choices=['ia32', 'x64'],
		help='target architecture. ia32 or x64.')
	parser.add_option(
		'-b', '--build',
		dest='build',
		default='Release',
		type='choice',
		choices=['Release', 'Debug'],
		help='target architecture. Debug or Release.')
	parser.add_option(
		'--vc-ver',
		dest='vc_ver',
		default='auto',
		type='choice',
		choices=['auto', '2005', '2008', '2010', '2012', '2012e'],
		help='target Visual Studio version. 2005, 2008, 2010, 2012 or 2012e.')
	option, args_ = parser.parse_args()
	
	if sys.platform.startswith('win32'):
		args = get_common_gyp_args(option)
		args.append('-G')
		args.append('msvs_version=' + option.vc_ver)
		run_gyp(args)
	else:
		print 'sorry, it works on win32 only.'
	
if __name__ == "__main__":
	main()
	
