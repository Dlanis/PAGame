project('fmt', 'cpp', version: '10.2.1')

headers = [
  'include/fmt/args.h',
  'include/fmt/chrono.h',
  'include/fmt/color.h',
  'include/fmt/compile.h',
  'include/fmt/core.h',
  'include/fmt/format-inl.h',
  'include/fmt/format.h',
  'include/fmt/os.h',
  'include/fmt/ostream.h',
  'include/fmt/printf.h',
  'include/fmt/ranges.h',
  'include/fmt/std.h',
  'include/fmt/xchar.h',
]

sources = [
  'src/format.cc',
  'src/os.cc',
]

inc = include_directories('./include')
defs = ['-DFMT_USE_FULL_CACHE_DRAGONBOX=1']

fmt = static_library('fmt', [headers, sources],
  include_directories: inc,
  cpp_args: defs,
  install: false)

fmt_dep = declare_dependency(
  include_directories: inc,
  compile_args: defs,
  link_with: fmt)
