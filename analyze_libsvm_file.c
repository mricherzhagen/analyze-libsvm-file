#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<math.h>

typedef unsigned int count_t;

int lenAsText(int n) {
	return ((int)log10(n))+1;
}

count_t *classcount;
int compare(const void * a, const void * b) {
	return classcount[*(int *)a] - classcount[*(int *)b];
}

int main(int argc, char *argv[]) {

	if (argc != 2) {
		printf("USAGE: %s [libsvm test/train file]\n", argv[0]);
		return 1;
	}
	FILE *fp = fopen(argv[1],"r");
	
	size_t max_classlabels=16, max_features=16;
	size_t used_classlabels = 0, used_features = 0;
	double *classlabels = calloc(sizeof(double),max_classlabels);
	count_t *classlabels_cnt = calloc(sizeof(count_t),max_classlabels);

	count_t *features_cnt = calloc(sizeof(count_t),max_features);
	count_t nsamples = 0;

	int i;
	int fstatus;
	count_t features_total = 0;
	while(1)
	{
		double label;
		fstatus = fscanf(fp,"%lf",&label);
		if (fstatus == EOF) break; 
		if (fstatus != 1) {
			printf("Error - can't read label\n");
			exit(1);
			break;
		}
		for (i = 0; i < used_classlabels; i++) {
			if (label == classlabels[i])
				break;
		}
		if (i >= used_classlabels) {
			//new label found
			if (used_classlabels == max_classlabels) {
				max_classlabels = 2*max_classlabels;
				classlabels = realloc(classlabels,sizeof(double)*max_classlabels);
				classlabels_cnt = realloc(classlabels_cnt,sizeof(count_t)*max_classlabels);
			}
			classlabels[used_classlabels] = label;
			classlabels_cnt[used_classlabels] = 1;
			used_classlabels+=1;
		} else {
			classlabels_cnt[i] += 1;
		}
		nsamples += 1;
		
	
		while(1)
		{
			int c;
			do {
				c = getc(fp);
			} while(isspace(c) && c != '\n');
			if (c == '\n' || c == EOF) break;
			ungetc(c,fp);
			int feature_nr;
			float feature_value;
			fscanf(fp,"%d:%f",&feature_nr, &feature_value);
			if (feature_value==0) continue;
			//feature_nr -= 1;
			features_total += 1;
			if (feature_nr >= max_features) {
				size_t new_size = feature_nr*2 ;
				features_cnt = realloc(features_cnt, new_size*sizeof(count_t));
				for (i=max_features; i < new_size;i++) {
					features_cnt[i] = 0;
				}
				max_features=new_size;
			}
			if (feature_nr > used_features) used_features = feature_nr;
				features_cnt[feature_nr] += 1;
		}
//		printf(".");
	}
	fclose(fp);

	int countlen = lenAsText(nsamples);
	if (lenAsText(features_total) > countlen) countlen = lenAsText(features_total);
	if (lenAsText(nsamples*used_features - features_total) > countlen) countlen = lenAsText(nsamples*used_features - features_total);


	//Sort classes
	int *class_idx = malloc(sizeof(int)*used_classlabels);
	classcount = classlabels_cnt;
	for (i=0; i<used_classlabels;i++) 
		class_idx[i] = i;
	qsort(class_idx,used_classlabels,sizeof(int),compare);


	printf("Classes: \n");
	int j;
	for (j = 0; j < used_classlabels;j++) {
		i=class_idx[j];
		printf("%5.1f %7.3f%% %*d\n", classlabels[i], classlabels_cnt[i]*100.0/nsamples,countlen,classlabels_cnt[i]);
	}
	printf("Features:\n");

	for (i=1; i < used_features;i++) {

		if (nsamples - features_cnt[i] != 0)
			printf("%5d %7.3f%% %*d %*d\n", i, features_cnt[i]*100.0/nsamples, countlen, features_cnt[i], countlen, nsamples - features_cnt[i]);
		else
			printf("%5d %7.3f%% %*d\n", i, features_cnt[i]*100.0/nsamples, countlen, features_cnt[i]);
	}
	printf("\n%5s %7.3f%% %*d %*ld\n","ALL",100.0*features_total/(used_features*nsamples),countlen,features_total,countlen,nsamples*used_features-features_total);
	return 0;
}
