﻿#
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

{
	'variables': {
		# build values.
		'library%': 'static_library',
		'build_type%': 'Debug',
		'coverage%': 'false',
		'ignore_all_default_libraries': 'false',

		# architecture values.
		'target_arch%': 'ia32',
	},
	
	# target_defaults for generic conditions.
	'target_defaults': {
		'default_configuration': '<(build_type)',
	},
	
	# target_defaults for each conditions.
	'conditions': [
		
		# target_defaults for Windows.
		[ 'OS == "win"', {
			'target_defaults': {
				'defines': [
					'WIN32',
					'_WINDOWS',
					'_CRT_SECURE_NO_DEPRECATE',
					'_WIN32_WINNT=0x501'
				],
				
				'msvs_configuration_attributes': {
					'CharacterSet': 1,
					'IntermediateDirectory': '$(SolutionDir)\\obj\\$(PlatformName)\\$(ConfigurationName)\\$(ProjectName)',
					'OutputDirectory': '$(SolutionDir)bin\\$(PlatformName)\\$(ConfigurationName)',
				},
				
				'msvs_settings': {
					'VCCLCompilerTool': {
						# General
						'DebugInformationFormat': 3, # Generate a PDB
						'WarningLevel': 3,
						'Detect64BitPortabilityProblems': 'true',
						'WarnAsError': 'false',

						# Optimizations

						# Preprocessors
						
						# Code Generations
						'StringPooling': 'true',
						'MinimalRebuild': 'false',
						'ExceptionHandling': 0,
						'BasicRuntimeChecks': 0,
						'BufferSecurityCheck': 'false',

						# Languages

						# Precompiled Headers
						'UsePrecompiledHeader': 0,

						# Outputs

						# Browser Informations

						# Advanced

						'AdditionalOptions': [
							 '/MP', # compile across multiple CPUs
						 ],
					},
					
					'VCLibrarianTool': {
						# General
						'OutputFile': '$(OutDir)\\lib\\$(ProjectName).lib',
						'IgnoreAllDefaultLibraries': '<(ignore_all_default_libraries)',
					},
					
					'VCLinkerTool': {
						# General
						'SuppressStartupBanner': 'true',

						# Inputs
						'IgnoreAllDefaultLibraries': '<(ignore_all_default_libraries)',

						# Manifests
						'GenerateManifest': 'false',
						'AllowIsolation': 'true',

						# Advanced
						'RandomizedBaseAddress': 2, # enable ASLR
						'DataExecutionPrevention': 2, # enable DEP
					},

					'VCManifestTool': {
						# Inputs and Outputs
						'EmbedManifest' : 'false',
					},
				},
				
				'configurations': {
					# configuration for 'Debug'.
					'Debug': {
						'defines': [
							'_DEBUG',
						],
						
						'msvs_settings': {
							'VCCLCompilerTool': {
								# Optimizations
								'Optimization': 0, # /Od, no optimization

								# Code Generations
								'RuntimeLibrary': 1,
							},

							'VCLinkerTool': {
								# General
								'GenerateDebugInformation': 'true',
							},
						},
					},
					
					# configuration for 'Release'.
					'Release': {
						'defines': [
							'NDEBUG',
						],
						
						'msvs_configuration_attributes': {
							'LinkTimeCodeGeneration': 1, # link-time code generation
						},

						'msvs_settings': {
							'VCCLCompilerTool': {
								# Optimizations
								'Optimization': 3, # /Ox, full optimization
								'InlineFunctionExpansion': 2, # /Ob2, inline anything eligible
								'EnableIntrinsicFunctions': 'true',
								'FavorSizeOrSpeed': 1, # /Ot, favour speed over size
								'OmitFramePointers': 'true',
								'WholeProgramOptimization': 'true', # /GL, whole program optimization, needed for LTCG

								# Code Generations
								'RuntimeLibrary': 0,
								'EnableFunctionLevelLinking': 'true',
							},

							'VCLibrarianTool': {
								'AdditionalOptions': [
									'/LTCG', # link time code generation
								],
							},
							
							'VCLinkerTool': {
								# General
								'GenerateDebugInformation': 'true',
								'LinkIncremental': 1, # disable incremental linking

								# Optimizations
								'OptimizeReferences': 2, # /OPT:REF
								'EnableCOMDATFolding': 2, # /OPT:ICF
								
								'AdditionalOptions': [
									'/LTCG', # link time code generation
								],
							},
						},
					},
				},
				
				# configuration for x64.
				'conditions': [
					[ 'target_arch == "x64"', {
						'defines': [ '_WIN64' ],
						'msvs_configuration_platform': 'x64',
						'msvs_settings': {
							'VCMIDLTool': {
								'TargetEnvironment': 3, # /env x64
							},
						},
						
						'configurations': {
							'Debug_x64': {
								'inherit_from': ['Debug'],
							},
							'Release_x64': {
								'inherit_from': ['Release'],
							},
						},
					}],
				],
			},
		}],
	],
}
