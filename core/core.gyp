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
				'include/mw32/base.h',
				'lib/mw32_base.c',
			],
		},
	],
}
