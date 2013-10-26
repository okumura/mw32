{
	'targets': [
		{
			'target_name': 'mw32core',
			'type': '<(library)',

			'all_dependent_settings': {
				'include_dirs': [
					'include',
				],
			},

			'include_dirs': [
				'include',
			],
			
			'sources': [
				'include/mw32.h',
				'include/mw32/defs.h',
				'include/mw32/hash.h',
				'include/mw32/module.h',
				'include/mw32/process.h',
				'include/mw32/thread.h',
				'include/mw32/types.h',
				'lib/mw32_hash.c',
				'lib/mw32_module.c',
				'lib/mw32_private.h',
				'lib/mw32_process.c',
				'lib/mw32_thread.c',
			],
		},
	],
}
