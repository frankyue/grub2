#! /usr/bin/ruby -w
#
# Copyright (C) 2002 Yoshinori K. Okuji <okuji@enbug.org>
#
# This genmk.rb is free software; the author
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY, to the extent permitted by law; without
# even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE.

module Enumerable
  def collect_with_index
    ret = []
    self.each_with_index do |item, index|
      ret.push(yield(item, index))
    end
    ret
  end
end

class String
  def to_var
    self.gsub(/[^a-zA-Z0-9_@]/, '_')
  end

  def suffix(str)
    self.sub(/\.[^\.]*$/, '') + '.' + str
  end

  def to_obj
    self.sub(/\.[^\.]*$/, '').to_var + '.o'
  end
end

class Image
  def initialize(dir, name)
    @dir = dir
    @name = name
  end
  attr_reader :dir, :name

  def rule(sources)
    prefix = @name.to_var
    exe = @name.suffix('exec')
    objs = sources.collect do |src|
      raise "unknown source file `#{src}'" if /\.[cS]$/ !~ src
      prefix + '-' + src.to_obj
    end
    objs_str = objs.join(' ')
    deps = objs.collect {|obj| obj.suffix('d')}
    deps_str = deps.join(' ')
    
    "CLEANFILES += #{@name} #{exe} #{objs_str}
MOSTLYCLEANFILES += #{deps_str}

#{@name}: #{exe}
	$(OBJCOPY) -O binary -R .note -R .comment $< $@

#{exe}: #{objs_str}
	$(CC) $(LDFLAGS) $(#{prefix}_LDFLAGS) -o $@ $^

" + objs.collect_with_index do |obj, i|
      src = sources[i]
      fake_obj = File.basename(src).suffix('o')
      dep = deps[i]
      flag = if /\.c$/ =~ src then 'CFLAGS' else 'ASFLAGS' end
      extra_flags = if /\.S$/ =~ src then '-DASM_FILE=1' else '' end
      dir = File.dirname(src)
      
      "#{obj}: #{src}
	$(CC) -I#{dir} -I$(srcdir)/#{dir} $(CPPFLAGS) #{extra_flags} $(#{flag}) $(#{prefix}_#{flag}) -c -o $@ $<

#{dep}: #{src}
	set -e; \
	  $(CC) -I#{dir} -I$(srcdir)/#{dir} $(CPPFLAGS) #{extra_flags} $(#{flag}) $(#{prefix}_#{flag}) -M $< \
	  | sed 's,#{Regexp.quote(fake_obj)}[ :]*,#{obj} $@ : ,g' > $@; \
	  [ -s $@ ] || rm -f $@

-include #{dep}

"
    end.join('')
  end
end

# Use PModule instead Module, to avoid name conflicting.
class PModule
  def initialize(dir, name)
    @dir = dir
    @name = name
  end
  attr_reader :dir, :name

  def rule(sources)
    prefix = @name.to_var
    objs = sources.collect do |src|
      raise "unknown source file `#{src}'" if /\.[cS]$/ !~ src
      prefix + '-' + src.to_obj
    end
    objs_str = objs.join(' ')
    deps = objs.collect {|obj| obj.suffix('d')}
    deps_str = deps.join(' ')
    pre_obj = 'pre-' + @name.suffix('o')
    mod_src = 'mod-' + @name.suffix('c')
    mod_obj = mod_src.suffix('o')
    defsym = 'def-' + @name.suffix('lst')
    undsym = 'und-' + @name.suffix('lst')
    mod_name = File.basename(@name, '.mod')
    
    "CLEANFILES += #{@name} #{mod_obj} #{mod_src} #{pre_obj} #{objs_str} #{defsym} #{undsym}
MOSTLYCLEANFILES += #{deps_str}
DEFSYMFILES += #{defsym}
UNDSYMFILES += #{undsym}

#{@name}: #{pre_obj} #{mod_obj}
	-rm -f $@
	$(LD) -r -o $@ $^
	$(STRIP) --strip-unneeded -K pupa_mod_init -K pupa_mod_fini -R .note -R .comment $@

#{pre_obj}: #{objs_str}
	-rm -f $@
	$(LD) -r -o $@ $^

#{mod_obj}: #{mod_src}
	$(CC) $(CPPFLAGS) $(CFLAGS) $(#{prefix}_CFLAGS) -c -o $@ $<

#{mod_src}: moddep.lst genmodsrc.sh
	sh $(srcdir)/genmodsrc.sh '#{mod_name}' $< > $@ || (rm -f $@; exit 1)

#{defsym}: #{pre_obj}
	$(NM) -g --defined-only -P -p $< | sed 's/^\\([^ ]*\\).*/\\1 #{mod_name}/' > $@

#{undsym}: #{pre_obj}
	echo '#{mod_name}' > $@
	$(NM) -u -P -p $< >> $@

" + objs.collect_with_index do |obj, i|
      src = sources[i]
      fake_obj = File.basename(src).suffix('o')
      dep = deps[i]
      flag = if /\.c$/ =~ src then 'CFLAGS' else 'ASFLAGS' end
      dir = File.dirname(src)

      "#{obj}: #{src}
	$(CC) -I#{dir} -I$(srcdir)/#{dir} $(CPPFLAGS) $(#{flag}) $(#{prefix}_#{flag}) -c -o $@ $<

#{dep}: #{src}
	set -e; \
	  $(CC) -I#{dir} -I$(srcdir)/#{dir} $(CPPFLAGS) $(#{flag}) $(#{prefix}_#{flag}) -M $< \
	  | sed 's,#{Regexp.quote(fake_obj)}[ :]*,#{obj} $@ : ,g' > $@; \
	  [ -s $@ ] || rm -f $@

-include #{dep}

"
    end.join('')
  end
end

class Utility
  def initialize(dir, name)
    @dir = dir
    @name = name
  end
  attr_reader :dir, :name

  def rule(sources)
    prefix = @name.to_var
    objs = sources.collect do |src|
      raise "unknown source file `#{src}'" if /\.c$/ !~ src
      prefix + '-' + src.to_obj
    end
    objs_str = objs.join(' ');
    deps = objs.collect {|obj| obj.suffix('d')}
    deps_str = deps.join(' ');

    "CLEANFILES += #{@name} #{objs_str}
MOSTLYCLEANFILES += #{deps_str}

#{@name}: #{objs_str}
	$(BUILD_CC) $(BUILD_LDFLAGS) $(#{prefix}_LDFLAGS) -o $@ $^

" + objs.collect_with_index do |obj, i|
      src = sources[i]
      fake_obj = File.basename(src).suffix('o')
      dep = deps[i]
      dir = File.dirname(src)

      "#{obj}: #{src}
	$(BUILD_CC) -I#{dir} -I$(srcdir)/#{dir} $(BUILD_CPPFLAGS) $(BUILD_CFLAGS) -DPUPA_UTIL=1 $(#{prefix}_CFLAGS) -c -o $@ $<

#{dep}: #{src}
	set -e; \
	  $(BUILD_CC) -I#{dir} -I$(srcdir)/#{dir} $(BUILD_CPPFLAGS) $(BUILD_CFLAGS) -DPUPA_UTIL=1 $(#{prefix}_CFLAGS) -M $< \
	  | sed 's,#{Regexp.quote(fake_obj)}[ :]*,#{obj} $@ : ,g' > $@; \
	  [ -s $@ ] || rm -f $@

-include #{dep}

"
    end.join('')
  end
end

images = []
utils = []
pmodules = []

cont = false
s = nil
while l = gets
  if cont
    s += l
  else
    s = l
  end

  print l
  cont = (/\\$/ =~ l)
  unless cont
    s.gsub!(/\\\n/, ' ')
    
    if /^([a-zA-Z0-9_]+)\s*=\s*(.*?)\s*$/ =~ s
      var, args = $1, $2

      if var =~ /^([a-zA-Z0-9_]+)_([A-Z]+)$/
	prefix, type = $1, $2

	case type
	when 'IMAGES'
	  images += args.split(/\s+/).collect do |img|
	    Image.new(prefix, img)
	  end

	when 'MODULES'
	  pmodules += args.split(/\s+/).collect do |pmod|
	    PModule.new(prefix, pmod)
	  end
	  
	when 'UTILITIES'
	  utils += args.split(/\s+/).collect do |util|
	    Utility.new(prefix, util)
	  end

	when 'SOURCES'
	  if img = images.detect() {|i| i.name.to_var == prefix}
	    print img.rule(args.split(/\s+/))
	  elsif pmod = pmodules.detect() {|m| m.name.to_var == prefix}
	    print pmod.rule(args.split(/\s+/))
	  elsif util = utils.detect() {|u| u.name.to_var == prefix}
	    print util.rule(args.split(/\s+/))
	  end
	end
      end
      
    end
    
  end
  
end

puts "CLEANFILES += moddep.lst"
puts "pkgdata_DATA += moddep.lst"
puts "moddep.lst: $(DEFSYMFILES) $(UNDSYMFILES) genmoddep"
puts "	cat $(DEFSYMFILES) /dev/null | ./genmoddep $(UNDSYMFILES) > $@ \\"
puts "	  || (rm -f $@; exit 1)"