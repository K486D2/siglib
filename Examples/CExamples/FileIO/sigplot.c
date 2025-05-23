// SigLib Fill array from disk hex file test program
// Copyright (c) 2024 Delta Numerix All rights reserved.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <siglib.h>       // SigLib DSP library
#include <gnuplot_c.h>    // Gnuplot/C

FILE* fpInputFile;

int main(int argc, char** argv)
{
  h_GPC_Plot* h2DPlot;    // Plot object

  if (argc != 2) {
    printf("\nUsage:\nsigplot filename (no extension)\n\n");
    exit(-1);    // Exit - usage error
  }

  h2DPlot =                           // Initialize plot
      gpc_init_2d("Plot File",        // Plot title
                  "Time",             // X-Axis label
                  "Magnitude",        // Y-Axis label
                  GPC_AUTO_SCALE,     // Scaling mode
                  GPC_SIGNED,         // Sign mode
                  GPC_KEY_ENABLE);    // Legend / key mode
  if (NULL == h2DPlot) {
    printf("\nPlot creation failure.\n");
    exit(-1);
  }

  printf("Source file = %s\n", argv[1]);

  SLArrayIndex_t inputSampleCount = SUF_SigFileLength(argv[1]);
  if (inputSampleCount == -1) {
    printf("Error opening input file to count number of samples: %s\n", argv[1]);
    exit(-1);
  }

  printf("Sample count = %d\n", inputSampleCount);

  SLData_t* pDataArray = SUF_VectorArrayAllocate(inputSampleCount);
  if (NULL == pDataArray) {
    printf("\n\nMemory allocation failed\n\n");
    exit(0);
  }

  inputSampleCount = SUF_SigReadFile(pDataArray, argv[1]);
  if (inputSampleCount == -1) {
    printf("Error reading from input file: %s\n", argv[1]);
    exit(-1);
  }

  if (!inputSampleCount) {
    printf("Can not read from .sig file %s\n", argv[1]);
    exit(0);
  }

  gpc_plot_2d(h2DPlot,                           // Graph handle
              pDataArray,                        // Dataset
              inputSampleCount,                  // Dataset length
              "Input Signal",                    // Dataset title
              (double)SIGLIB_ZERO,               // Minimum X value
              (double)(inputSampleCount - 1),    // Maximum X value
              "lines",                           // Graph type
              "magenta",                         // Colour
              GPC_NEW);                          // New graph

  getchar();

  free(pDataArray);    // Free memory

  return (0);
}
