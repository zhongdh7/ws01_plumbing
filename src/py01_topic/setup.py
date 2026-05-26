from glob import glob

from setuptools import find_packages, setup

package_name = 'py01_topic'

setup(
    name=package_name,
    version='0.0.0',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
        ('share/' + package_name + '/launch', glob('launch/*.launch.py')),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='loser',
    maintainer_email='loser@todo.todo',
    description='TODO: Package description',
    license='TODO: License declaration',
    extras_require={
        'test': [
            'pytest',
        ],
    },
    entry_points={
        'console_scripts': [
            'demo_str_py_publisher = py01_topic.demo_str_py_publisher:main',
            "demo_str_py_listener = py01_topic.demo_str_py_listener:main",
            "demo_stu_py_publisher = py01_topic.demo_stu_py_publisher:main",
            "demo_stu_py_listener = py01_topic.demo_stu_py_listener:main",
        ],
    },
)
