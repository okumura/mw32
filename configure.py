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
	
