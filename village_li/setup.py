from setuptools import setup, find_packages

package_name = 'village_li'

setup(
    name=package_name,
    version='0.0.0',
    packages=find_packages(exclude=['test']),
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='huang-ruihong',
    maintainer_email='huang-ruihong@shturl.',
    description='village li demo',
    license='Apache-2.0',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
            'li4_node = village_li.li4:main',
        ],
    },
)

