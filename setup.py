from distutils.core import setup, Extension

module = Extension('serial',
		   define_macros = [('MAJOR_VERSION', '1'),
				    ('MINOR_VERSION', '0')],
		   include_dirs = ['./header', '/usr/include'],
		   sources = ['./src/libSerial.c'])

setup(name = 'Serial',
      version = '1.0',
      description = 'Basic communication from elegoo to raspberry pi.',
      author = 'Paul Becker',
      author_email = 'paul.becker@tu-dortmund.de',
      ext_modules = [module])
