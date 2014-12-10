analyze-libsvm-file
===================

Outputs some statistics about how many features and classes are present in the libsvm training/test data file.

Example output for the `heart_scale` file that is distributed with libsvm:
```
Classes: 
  1.0  44.444%  120
 -1.0  55.556%  150
Features:
    #   Count%   Count n-count        Min       Mean        Max   Variance
    1  97.407%     263       7  -1.000000   0.061312   1.000000   0.147243
    2 100.000%     270          -1.000000   0.355556   1.000000   0.873580
    3 100.000%     270          -1.000000   0.449383   1.000000   0.399701
    4 100.000%     270          -1.000000  -0.295388   1.000000   0.113156
    5 100.000%     270          -1.000000  -0.435346   1.000000   0.055495
    6 100.000%     270          -1.000000  -0.703704   1.000000   0.504801
    7  99.259%     268       2  -1.000000   0.022388   1.000000   0.999499
    8 100.000%     270          -1.000000   0.201187   1.000000   0.124623
    9 100.000%     270          -1.000000  -0.340741   1.000000   0.883896
   10  99.630%     269       1  -1.000000  -0.663749   1.000000   0.134841
   11  54.815%     148     122  -1.000000  -0.756757   1.000000   0.427319
   12 100.000%     270          -1.000000  -0.553086   1.000000   0.394507
   13 100.000%     270          -1.000000  -0.151852   1.000000   0.938052

  ALL  96.239%    3378     132  -1.000000  -0.216215   1.000000   0.461286

```

How to read the output:
-----------------------
Classes:
`1.0  44.444%  120` means, there are `120` samples (`44.444%` of all samples) in the class with the label `1.0`

Features: 
`1  97.407%  263    7` means, that `263` samples contain the feature and `7` samples don't contain it. 
So `97.407%` of the samples contain feature `1`. 
If all samples contain the feature the number of samples that are missing the feature is omited.

The last four numbers show the minimum, average, maximum and variance of the feature values.

The last line is a summary for all the features. 
The count%, minimum, average, maximum and variance are averaged.
