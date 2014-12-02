import sys
import re
import collections
import math

if len(sys.argv) != 2:
	print "USAGE: %s [libsvm train/test file]" % sys.argv[0]
	sys.exit(1)
	
lineRex = re.compile(r"^(\d+)\s+(.+)$")
#featureRex = re.compile(r"(\d+):(\d+([\.,](\d+)?)?)\s+")
featureRex = re.compile(r"(\d+):")
linenumber = 0

debug = False

nsamples = 0
with open(sys.argv[1]) as h:
	classlabels = collections.defaultdict(int)
	features =  collections.defaultdict(int)
	for line in h:
		m = lineRex.match(line)
		if m:
			nsamples += 1
			label = int(m.group(1))
			featurestring = m.group(2)
			classlabels[label]+=1
			if debug:
				print "Featurestring: %s" % featurestring
			featureCount=0
			for f in featureRex.finditer(featurestring):
				featureCount+=1
				fid = int(f.group(1))
				features[fid]+=1
				if debug:
					print "Found %d:%s" % (fid,f.group(2))
			if featureCount != featurestring.count(':'):
				print "ERR!!!!!"	
			
		else:
			print("warning: ignoring line %d" % linenumber)
		if debug:
			raw_input(">")
		linenumber+=1

stellenClass = int(max(math.log(len(classlabels),10),math.log(len(features),10))) + 1
stellenCount = int(math.log(nsamples,10)) + 1
formatOhne =  "%"+str(stellenClass)+"d %7.3f%% %"+str(stellenCount)+"d "
format = formatOhne + "%"+str(stellenCount)+"d"

print "Classes:"	
for l,c in classlabels.items():
	print formatOhne % (l, 100.0*c/nsamples, c )
print "Features:"
sumFeatureCount = 0
for f,c in features.items():
	sumFeatureCount += c
	if nsamples - c  != 0:
		print format % (f, 100.0*c/nsamples,c,nsamples - c)
	else:
		print formatOhne % (f, 100.0*c/nsamples,c)


print "Feature Density: %7.4f" % (100.0*sumFeatureCount/(nsamples*len(features)))
