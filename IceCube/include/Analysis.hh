#ifndef Analysis_H
#define Analysis_H 1

#include "globals.hh"
#include "g4root.hh"
#include "Analysis.hh"

class Analysis {

  public:

    Analysis() {};
    virtual ~Analysis() {};

    // Acquire instance
    static Analysis* GetAnalysis()
    { static Analysis the_analysis; return &the_analysis; }

    void SetNumEvents(G4int numberofevents){ numevents = numberofevents; }
    int GetNumEvents(){ return numevents; }

    // Functions to append, call, and reset histograms and squared histograms (tally^2 per bin)
    void appendOpticalHistogram(G4int histogramBin) { eventOpticalHistogram[histogramBin]++; }
    void tallyOpticalHistograms(G4int numEvents) {
      for (i=0; i<100; i++) {
        //opticalHistogram[i] += eventOpticalHistogram[i];
        averageOpticalHistogram[i] += ((float)(eventOpticalHistogram[i]))/((float)(numEvents));
        //squaredOpticalHistogram[i] += pow(eventOpticalHistogram[i], 2);
        varianceOpticalHistogram[i] += (((float)(pow(eventOpticalHistogram[i], 2)))/((float)(numEvents)));
      }
    }
    //int callOpticalHistogram(G4int histogramBin) { return opticalHistogram[histogramBin]; }
    //int callSquaredOpticalHistogram(G4int histogramBin) { return squaredOpticalHistogram[histogramBin]; }
    double callAverageOpticalHistogram(G4int histogramBin) { return averageOpticalHistogram[histogramBin]; }
    double callVarianceOpticalHistogram(G4int histogramBin, G4int numEvents) {
      varianceOpticalHistogram[histogramBin] = ((float)(numEvents))*(varianceOpticalHistogram[histogramBin] - pow(averageOpticalHistogram[histogramBin], 2));
      return varianceOpticalHistogram[histogramBin];
    }
    //void resetOpticalHistogram() { // Not necessary on a per-run basis
    //  for (i=0; i<100; i++) {
    //    opticalHistogram[i] = 0;
    //    squaredOpticalHistogram[i] = 0;
    //  }
    //}
    void resetEventOpticalHistogram() {
      for (i=0; i<100; i++) {
        eventOpticalHistogram[i] = 0;
      }
    }
    
  private:
    G4int i, numevents, eventOpticalHistogram[100]; //, opticalHistogram[100], squaredOpticalHistogram[100];
    G4double averageOpticalHistogram[100], varianceOpticalHistogram[100];

};

#endif
