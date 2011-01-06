import sys
import re
import os

sohead = re.compile('(.+\.so):')
funchead = re.compile('([0-9a-f]{8}) <(.+)>:')
funcline = re.compile('^[ ]+([0-9a-f]+):.+')

def parsestack( lines, libname ):
    crashline = re.compile('.+pc.([0-9a-f]{8}).+%s' % libname )
    ret = []
    for l in lines:
        m = crashline.match(l)
        if m:
            addr =  m.groups()[0]
            ret.append(int(addr,16))
    return ret

def parseasm( lines ):
    ret = []
    current = None
    for l in lines:
        m = funchead.match(l)
        if m:
            if current:
                ret.append(current)
            startaddr, funcname =  m.groups()
            current = [ funcname, int(startaddr,16), int(startaddr,16) ]
        m = funcline.match(l)
        if m:
            addr =  m.groups()[0]
            if current != None:
                current[2] = int(addr,16)
        m = sohead.match(l)
        if m:
            so =  m.groups()[0]
            so = os.path.split(so)[1]
    return so, ret

if __name__=="__main__":
    asm, stack = sys.argv[1],sys.argv[2]

    libname, asm = parseasm( file(asm).read().split('\n') )
    stack = parsestack( file(stack).read().split('\n'), libname )

    for addr in stack:
        for func, a1, a2 in asm:
            if addr >= a1 and addr <= a2:
                print "0x%08x:%32s + 0x%04x" % ( addr, func, addr-a1 )
