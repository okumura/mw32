{
	'targets': [
		{
			'target_name': 'mw32',
			'type': 'none',
			
			'conditions': [
				[ 'OS == "win"', {
					'dependencies': [
						'core/core.gyp:*',
					],
				}],
			],
		},
	],
}
