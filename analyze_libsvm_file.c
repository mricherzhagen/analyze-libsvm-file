#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<math.h>
#include<locale.h>

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
	double *feature_mean = calloc(sizeof(double),max_features);
	double *feature_m2 = calloc(sizeof(double),max_features);
	float *feature_min = calloc(sizeof(float),max_features);
	float *feature_max = calloc(sizeof(float),max_features);
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
				feature_mean = realloc(feature_mean, new_size*sizeof(double));
				feature_m2 = realloc(feature_m2, new_size*sizeof(double));
				feature_min = realloc(feature_min, new_size*sizeof(float));
				feature_max = realloc(feature_max, new_size*sizeof(float));
				for (i=max_features; i < new_size;i++) {
					features_cnt[i] = 0;
					feature_mean[i] = 0;
					feature_m2[i] = 0;
					feature_min[i] = INFINITY;
					feature_max[i] = -INFINITY;
				}
				max_features=new_size;
			}
			if (feature_nr > used_features) used_features = feature_nr;
			//Online Algorithm for Variance & Mean. Src: http://en.wikipedia.org/wiki/Algorithms_for_calculating_variance#Incremental_algorithm
			//n=features_cnt[feature_nr], mean=feature_mean[feature_nr], M2 = feature_m2[feature_nr] 
			features_cnt[feature_nr] += 1;
			double delta = feature_value - feature_mean[feature_nr];
			feature_mean[feature_nr] += delta/features_cnt[feature_nr];
			feature_m2[feature_nr] += delta*(feature_value - feature_mean[feature_nr]);
			if (feature_value > feature_max[feature_nr]) feature_max[feature_nr] = feature_value;
			if (feature_value < feature_min[feature_nr]) feature_min[feature_nr] = feature_value;
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
	countlen = countlen < 7 ? 7 : countlen;
	printf("%5s %8s %*s %*s %10s %10s %10s %10s\n", "#", "Count%", countlen, "Count", countlen, "n-count", "Min", "Mean", "Max", "Variance");
	double minsum = 0;
	double maxsum = 0;
	double meansum = 0;
	double variancesum = 0;
	for (i=1; i <= used_features;i++) {

		minsum += feature_min[i];
		maxsum += feature_max[i];
		meansum += feature_mean[i];
		double feature_variance = feature_m2[i]/(features_cnt[i] - 1);
		variancesum += feature_variance;
		printf("%5d %7.3f%% %*d ", i, features_cnt[i]*100.0/nsamples, countlen, features_cnt[i]);
		if (nsamples - features_cnt[i] != 0)
			printf("%*d ",  countlen, nsamples - features_cnt[i]);
		else
			printf("%*s ", countlen, "" );
		printf("%10f %10f %10f %10f\n", feature_min[i], feature_mean[i], feature_max[i], feature_variance);
	}
	printf("\n%5s %7.3f%% %*d %*ld","ALL",100.0*features_total/(used_features*nsamples),countlen,features_total,countlen,nsamples*used_features-features_total);
	printf(" %10f %10f %10f %10f\n", minsum/used_features, meansum/used_features, maxsum/used_features,variancesum/used_features);

	if (used_classlabels >= 2) {
		count_t biggest_classifier_count = classlabels_cnt[class_idx[used_classlabels - 1]] + classlabels_cnt[class_idx[used_classlabels - 2]];
		size_t bytes = biggest_classifier_count*biggest_classifier_count*sizeof(double);
		setlocale(LC_ALL,"");
		printf("\nFull Kernel Matrix Size: %'dx%'d doubles =  %'d Bytes = %'.2f KByte = %'.2f MByte\n", biggest_classifier_count, biggest_classifier_count, bytes, bytes/1024.0, bytes/1024.0/1024.0);
	}

	return 0;
}
