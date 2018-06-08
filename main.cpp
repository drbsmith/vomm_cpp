/**
 @file
 ml_vomm - Fuzzy C-means clustering implementation, ©2017 Benjamin Smith
    based on https://en.wikipedia.org/wiki/Fuzzy_clustering
 
 @ingroup	ml.*
 */

#include "ext.h"
#include "ext_obex.h"
#include "ext_strings.h"
#include "ext_common.h"

#include <vector>
using namespace std;

// a wrapper for cpost() only called for debug builds on Windows
// to see these console posts, run the DbgView program (part of the SysInternals package distributed by Microsoft)
#if defined( NDEBUG ) || defined( MAC_VERSION )
#define DPOST
#else
#define DPOST cpost
#endif

//// a macro to mark exported symbols in the code without requiring an external file to define them
//#ifdef WIN_VERSION
//// note that this is the required syntax on windows regardless of whether the compiler is msvc or gcc
//#define T_EXPORT __declspec(dllexport)
//#else // MAC_VERSION
//// the mac uses the standard gcc syntax, you should also set the -fvisibility=hidden flag to hide the non-marked symbols
//#define T_EXPORT __attribute__((visibility("default")))
//#endif

//#ifdef MAC_VERSION
//#include "ext_strings.h"
//#endif

#include "ml_vomm.hpp"

typedef vector<string>  stringVector;

typedef struct ml_vomm {
    t_object i_ob;
//    long m_in;  // inlet number used by proxies
    void *outlet, *out2; // the right outlet
    
//    int max_epochs; // set training limits in terms of repetitions and amount of residual error
//    float max_error;
////    void *m_proxy;
//    int numStates, numAlphabet, maxObservations;
//
//    int dynamic;    // bool really, but int for ATTR interfacing
    
//    int currentState;
    int debug;
    
    mlVOMM* myVOMM;
} t_ml_vomm;

BEGIN_USING_C_LINKAGE
void ml_vomm_assist(t_ml_vomm *x, void *b, long m, long a, char *s);
void ml_vomm_bang(t_ml_vomm *x);
void ml_vomm_list(t_ml_vomm *x, t_symbol *s, short ac, t_atom *av);
void *ml_vomm_new(); //double fuzz, long clusterCount, double threshold);
void ml_vomm_free(t_ml_vomm *x);
void ml_vomm_reset(t_ml_vomm *x);
//void ml_vomm_clearObs(t_ml_vomm *x);
//void ml_vomm_build(t_ml_vomm *x);

//void ml_vomm_read(t_ml_vomm *x, t_symbol *s);
//void ml_vomm_bw(t_ml_vomm *x); //, long s);
//void ml_vomm_backward(t_ml_vomm *x); //, long s);
//void ml_vomm_viterbi(t_ml_vomm *x); //, long s);
////
void ml_vomm_int(t_ml_vomm *x, long l);   // int is considered an observation
//void ml_vomm_int2(t_ml_vomm *x, long l);   // 2nd inlet, training states in
//void ml_vomm_setNumStates(t_ml_vomm* x, t_object *attr, long argc, t_atom *argv);
//t_max_err ml_vomm_getNumStates(t_ml_vomm *x, t_object *attr, long *argc, t_atom **argv);
//void ml_vomm_setAlphaSize(t_ml_vomm* x, t_object *attr, long argc, t_atom *argv);
//t_max_err ml_vomm_getAlphaSize(t_ml_vomm *x, t_object *attr, long *argc, t_atom **argv);
//void ml_vomm_setDynamic(t_ml_vomm* x, t_object *attr, long argc, t_atom *argv);
//t_max_err ml_vomm_getDynamic(t_ml_vomm *x, t_object *attr, long *argc, t_atom **argv);
//void ml_vomm_setDebug(t_ml_vomm* x, t_object *attr, long argc, t_atom *argv);
//t_max_err ml_vomm_getDebug(t_ml_vomm *x, t_object *attr, long *argc, t_atom **argv);
//void ml_vomm_setMaxObs(t_ml_vomm* x, t_object *attr, long argc, t_atom *argv);
//t_max_err ml_vomm_getMaxObs(t_ml_vomm *x, t_object *attr, long *argc, t_atom **argv);
////
//void ml_vomm_getStates(t_ml_vomm* x);
//void ml_vomm_getAlphabet(t_ml_vomm* x);
//void ml_vomm_dump(t_ml_vomm* x);
//void ml_vomm_getEmissions(t_ml_vomm* x);
//void ml_vomm_getTransitions(t_ml_vomm* x);
//void ml_vomm_getEmission(t_ml_vomm* x, long column);
//void ml_vomm_getTransition(t_ml_vomm* x, long row);
//void ml_vomm_getInitial(t_ml_vomm* x);
void ml_vomm_logEval(t_ml_vomm *x, t_symbol *s);
void ml_vomm_predict(t_ml_vomm *x, long i, t_symbol *s);
//void ml_vomm_logEval(t_ml_vomm *x, t_symbol *s, short ac, t_atom *av);
//void ml_vomm_setTransitions(t_ml_vomm *x, t_symbol *s, short ac, t_atom *av);
//void ml_vomm_setInitial(t_ml_vomm *x, t_symbol *s, short ac, t_atom *av);
END_USING_C_LINKAGE

// globals
static t_class *ml_vomm_class = NULL;

void ext_main(void *r)
{
	t_class *c;

    c = class_new("ml.vomm", (method)ml_vomm_new, (method)ml_vomm_free, sizeof(t_ml_vomm), 0L, 0); // A_DEFFLOAT, A_DEFLONG, A_DEFFLOAT, 0);
	class_addmethod(c, (method)ml_vomm_bang, "bang", 0);
	class_addmethod(c, (method)ml_vomm_list, "list", A_GIMME, 0);
	class_addmethod(c, (method)ml_vomm_assist, "assist", A_CANT, 0);
    
	class_addmethod(c, (method)ml_vomm_reset,	"reset",		0);
    class_addmethod(c, (method)ml_vomm_reset,	"clear",		0);
//    class_addmethod(c, (method)ml_vomm_clearObs,    "clear_observations",        0);
//
//    class_addmethod(c, (method)ml_vomm_build,    "build",        0);
//
    class_addmethod(c, (method)ml_vomm_int,        "int",        A_LONG, 0);    // the method for an int in the left inlet (inlet 0)
//    class_addmethod(c, (method)ml_vomm_int2,        "in1",        A_LONG, 0);    // the method for an int in the right inlet (inlet 1)
//
//    class_addmethod(c, (method)ml_vomm_dump,             "dump",         0);
//
//    class_addmethod(c, (method)ml_vomm_getEmission,     "get_emission",    A_LONG, 0);
//    class_addmethod(c, (method)ml_vomm_getTransition,   "get_transition",  A_LONG, 0);
//    class_addmethod(c, (method)ml_vomm_getEmissions,     "get_emissions",    0);
//    class_addmethod(c, (method)ml_vomm_getTransitions,   "get_transitions",  0);
//    class_addmethod(c, (method)ml_vomm_getInitial,       "get_initial", 0);
//
//    class_addmethod(c, (method)ml_vomm_getStates,        "get_states", 0);
//    class_addmethod(c, (method)ml_vomm_getAlphabet,      "get_alphabet", 0);
//
    class_addmethod(c, (method)ml_vomm_logEval,     "logeval",    A_SYM, 0);
    class_addmethod(c, (method)ml_vomm_predict,     "predict",    A_LONG, A_SYM, 0);
//    class_addmethod(c, (method)ml_vomm_setTransitions,   "transitions",  A_GIMME, 0);
//    class_addmethod(c, (method)ml_vomm_setInitial,       "initial",      A_GIMME, 0);
//
//    class_addmethod(c, (method)ml_vomm_bw,               "baum_welch",          0);
//    class_addmethod(c, (method)ml_vomm_bw,               "train",          0);
//    class_addmethod(c, (method)ml_vomm_backward,         "backward",   0);
//    class_addmethod(c, (method)ml_vomm_viterbi,          "viterbi",   0);
//    class_addmethod(c, (method)ml_vomm_viterbi,          "predict",   0);
//
//    CLASS_ATTR_LONG(c,              "epochs",   0, t_ml_vomm, max_epochs);
//    CLASS_ATTR_DEFAULT(c,    "epochs",   0,     "0");
//    CLASS_ATTR_FILTER_MIN(c,        "epochs",   0);
//
//    CLASS_ATTR_FLOAT(c,      "error",   0, t_ml_vomm, max_error);
//    CLASS_ATTR_DEFAULT(c,    "error",   0,     "0.001");
//    CLASS_ATTR_FILTER_MIN(c, "error",   0);
//
//    CLASS_ATTR_LONG(c,      "states",  0, t_ml_vomm, numStates);
//    CLASS_ATTR_FILTER_MIN(c, "states", 1);
//    CLASS_ATTR_ACCESSORS(c, "states",  ml_vomm_getNumStates, ml_vomm_setNumStates);    // don't need a custom getter, only setter
//
//    CLASS_ATTR_LONG(c,      "alphabet",  0, t_ml_vomm, numAlphabet);
//    CLASS_ATTR_FILTER_MIN(c, "alphabet", 1);
//    CLASS_ATTR_ACCESSORS(c, "alphabet",  ml_vomm_getAlphaSize, ml_vomm_setAlphaSize);    // don't need a custom getter, only setter
//
//    CLASS_ATTR_LONG(c,      "dynamic",  0, t_ml_vomm, dynamic);
//    CLASS_ATTR_DEFAULT(c,   "dynamic", 0, "0");
//    CLASS_ATTR_FILTER_CLIP(c, "dynamic", 0, 1);
//    CLASS_ATTR_ACCESSORS(c, "dynamic",  ml_vomm_getDynamic, ml_vomm_setDynamic);    // don't need a custom getter, only setter
//
//    CLASS_ATTR_LONG(c,      "debug",  0, t_ml_vomm, debug);  // print debug messages on/off
//    CLASS_ATTR_FILTER_CLIP(c, "debug", 0, 1);
//    CLASS_ATTR_ACCESSORS(c, "debug",  ml_vomm_getDebug, ml_vomm_setDebug);    // don't need a custom getter, only setter
//
//    CLASS_ATTR_LONG(c,      "max_observations",  0, t_ml_vomm, maxObservations);
//    CLASS_ATTR_DEFAULT(c,   "max_observations", 0, "65535");
//    CLASS_ATTR_FILTER_MIN(c, "max_observations", 0);
//    CLASS_ATTR_ACCESSORS(c, "max_observations",  ml_vomm_getMaxObs, ml_vomm_setMaxObs);    // don't need a custom getter, only setter
    
	class_register(CLASS_BOX, c);
	ml_vomm_class = c;
}

void *ml_vomm_new() //double fuzz, long clusterCount, double threshold)
{
    t_ml_vomm *x;
    
    x = (t_ml_vomm*)object_alloc(ml_vomm_class);
    if (x) {
        x->out2 = outlet_new(x,0L); // right outlet
        x->outlet = outlet_new(x, 0L);  // left outlet
        
        x->myVOMM = new mlVOMM(x);
        
//        x->currentState = -1;
//        x->max_error = 0.001f;
        
//        intin(x,1);
        
        ml_vomm_reset(x);
    }
    
    return x;
}
void ml_vomm_free(t_ml_vomm *x)
{
    if (x) {
        delete x->myVOMM;
    }
}

void ml_vomm_assist(t_ml_vomm *x, void *b, long m, long a, char *s)
{
    if (m == ASSIST_INLET) {
        switch (a) {
            case 0:
                sprintf(s,"observed tokens in");
                break;
            case 1:
                sprintf(s,"observed states in");
                break;
        }
    } else {
        switch (a) {
            case 0:
                sprintf(s,"generated tokens and predicted state");
                break;
            case 1:
                sprintf(s,"various messages");
                break;
        }
    }
}

void ml_vomm_bang(t_ml_vomm *x)
{
}

// list: observations, will be associated with 'currentState', set in right inlet
void ml_vomm_list(t_ml_vomm *x, t_symbol *s, short ac, t_atom *av)
{
    t_atom_long* data = new t_atom_long[ac];
    
    atom_getlong_array(ac, av, ac, data);
    
//    for (int i = 0; i < ac; i++)
//        x->myVOMM->AddObsAndTraining((t_object*)x, (int)data[i], x->currentState);
    
    delete[] data;
    
//    x->currentState = -1;   // reset training state
}

void ml_vomm_reset(t_ml_vomm* x)
{
//    x->myVOMM->reset();
}
//void ml_vomm_clearObs(t_ml_vomm *x)
//{
//    x->myVOMM->ClearObs();
//}
//void ml_vomm_build(t_ml_vomm* x)
//{
//    x->myVOMM->Buildvomm();
//}
void ml_vomm_int(t_ml_vomm *x, long l)   // int is considered an observation
{
//    if (x->currentState >= 0)
//        x->myVOMM->AddObsAndTraining((t_object*)x, l, x->currentState);
//    else
//        x->myVOMM->AddObservations((t_object*)x, l);
//
//    x->numAlphabet = x->myVOMM->GetAlphabetSize();
//    x->numStates = x->myVOMM->GetNumStates();

//    x->currentState = -1;   // clear training state
}

void ml_vomm_logEval(t_ml_vomm *x, t_symbol *s) {
    x->myVOMM->logEval(s->s_name);
}
void ml_vomm_predict(t_ml_vomm *x, long i, t_symbol *s) {
    x->myVOMM->predict(i, s->s_name);
}
//void ml_vomm_int2(t_ml_vomm *x, long l)   // second inlet: set training state to be associated with any input in left inlet
//{
//    if (l >= 0)
//        x->currentState = l;
//}
//
//void ml_vomm_bw(t_ml_vomm *x) //, long s)
//{
//    int count = 0;
//    double delta = -1;
//    do {
//        delta = x->myVOMM->BaumWelch();
//        count++;
//    } while ( count < x->max_epochs && delta > x->max_error );  // repeat until one of the escape conditions is met
//
////    post("%d < %d, %f > %f", count, x->max_epochs, delta, x->max_error);
//
//    t_atom out[2];
//    atom_setsym(&out[0], gensym("delta"));
//    atom_setfloat(&out[1], delta);
//
//    outlet_atoms(x->out2, 2, &out[0]);
//}
//void ml_vomm_backward(t_ml_vomm *x)
//{
//    x->myVOMM->Backward();
//}
//void ml_vomm_viterbi(t_ml_vomm *x)
//{
//    int state = x->myVOMM->Viterbi(x->out2);
//
//    outlet_int(x->outlet, state);
//}
//void ml_vomm_setNumStates(t_ml_vomm *x, t_object *attr, long argc, t_atom *argv) {
//    x->numStates = (int)atom_getlong(argv);
//    x->myVOMM->SetNumStates(x->numStates);
//    x->numStates = x->myVOMM->GetNumStates();
//}
//t_max_err ml_vomm_getNumStates(t_ml_vomm *x, t_object *attr, long *argc, t_atom **argv) {
//    char alloc;
//    atom_alloc(argc, argv, &alloc);     // allocate return atom
//
//    atom_setlong(*argv, x->myVOMM->GetNumStates());
//    return 0;
//}
//void ml_vomm_getStates(t_ml_vomm* x) {
//    int states = x->myVOMM->GetNumStates();
//
//    t_atom out[2];
//    atom_setsym(&out[0], gensym("states"));
//    atom_setlong(&out[1], states);
//
//    outlet_atoms(x->out2, 2, &out[0]);
//}
//void ml_vomm_setAlphaSize(t_ml_vomm* x, t_object *attr, long argc, t_atom *argv) {
//    x->numAlphabet = (int)atom_getlong(argv);
//    x->myVOMM->SetAlphabetSize(x->numAlphabet);
//    x->numAlphabet = x->myVOMM->GetAlphabetSize();
//}
//t_max_err ml_vomm_getAlphaSize(t_ml_vomm *x, t_object *attr, long *argc, t_atom **argv)  {
//    char alloc;
//    atom_alloc(argc, argv, &alloc);     // allocate return atom
//
//    atom_setlong(*argv, x->myVOMM->GetAlphabetSize());
//    return 0;
//}
//void ml_vomm_getAlphabet(t_ml_vomm* x) {
//    int alpha = x->myVOMM->GetAlphabetSize();
//
//    t_atom out[2];
//    atom_setsym(&out[0], gensym("alphabet"));
//    atom_setlong(&out[1], alpha);
//
//    outlet_atoms(x->out2, 2, &out[0]);
//}
//void ml_vomm_setDynamic(t_ml_vomm* x, t_object *attr, long argc, t_atom *argv) {
//    x->dynamic = (int)atom_getlong(argv);
//    x->myVOMM->SetDynamic(x->dynamic == 1);
//}
//t_max_err ml_vomm_getDynamic(t_ml_vomm *x, t_object *attr, long *argc, t_atom **argv) {
//    char alloc;
//    atom_alloc(argc, argv, &alloc);     // allocate return atom
//
//    atom_setlong(*argv, x->myVOMM->GetDynamic());
//    return 0;
//}
//void ml_vomm_setDebug(t_ml_vomm* x, t_object *attr, long argc, t_atom *argv) {
//    x->debug = (int)atom_getlong(argv);
//    x->myVOMM->SetVerbose(x->debug == 1);
//}
//t_max_err ml_vomm_getDebug(t_ml_vomm *x, t_object *attr, long *argc, t_atom **argv) {
//    char alloc;
//    atom_alloc(argc, argv, &alloc);     // allocate return atom
//
//    atom_setlong(*argv, x->myVOMM->GetVerbose());
//    return 0;
//}
//void ml_vomm_setMaxObs(t_ml_vomm* x, t_object *attr, long argc, t_atom *argv) {
//    x->maxObservations = (int)atom_getlong(argv);
//    x->myVOMM->SetMaxObservations(x->maxObservations);   // can't be less than 0, set in ARG define and checked in myVOMM
//}
//t_max_err ml_vomm_getMaxObs(t_ml_vomm *x, t_object *attr, long *argc, t_atom **argv) {
//    char alloc;
//    atom_alloc(argc, argv, &alloc);     // allocate return atom
//
//    atom_setlong(*argv, x->myVOMM->GetMaxObservations());
//    return 0;
//}
//void ml_vomm_setEmissions(t_ml_vomm *x, t_symbol *s, short ac, t_atom *av) {
//    int state;
//
//    int t = atom_gettype(av);
//    if (t == A_LONG) {
//        state = (int)atom_getlong(av++);
//
//        double *data = new double[ac-1];
//
//        for (int i = 1; i < ac; i++, av++) {
//            t = atom_gettype(av);
//            switch (t) {
//                case A_FLOAT:
//                    data[i-1] = atom_getfloat(av);
//                    break;
//                case A_LONG:
//                    data[i-1] = (double)atom_getlong(av);
//                    break;
//            }
//        }
//
//        x->myVOMM->SetEmissions(state, &data[0], ac-1);
//
//        delete[] data;
//    } else {
//        object_post((t_object*)x, "first element of list must be the state to set (int).");
//    }
//}
//void ml_vomm_setTransitions(t_ml_vomm *x, t_symbol *s, short ac, t_atom *av) {
//    int state;
//
//    t_atom_long t = atom_gettype(av);
//    if (t == A_LONG) {
//        state = (int)atom_getlong(av++);
//
//        double* data = new double[ac-1];
//
//        for (int i = 1; i < ac; i++, av++) {
//            t = atom_gettype(av);
//            switch (t) {
//                case A_FLOAT:
//                    data[i-1] = atom_getfloat(av);
//                    break;
//                case A_LONG:
//                    data[i-1] = (double)atom_getlong(av);
//                    break;
//            }
//        }
//
//        x->myVOMM->SetTransitions(state, &data[0], ac-1);
//
//        delete[] data;
//    } else {
//        object_post((t_object*)x, "first element of list must be the state to set (int).");
//    }
//}
//void ml_vomm_setInitial(t_ml_vomm *x, t_symbol *s, short ac, t_atom *av) {
//    double* data = new double[ac];
//
//    for (int i = 0; i < ac; i++, av++) {
//        int t = atom_gettype(av);
//        switch (t) {
//            case A_FLOAT:
//                data[i-1] = atom_getfloat(av);
//                break;
//            case A_LONG:
//                data[i-1] = (double)atom_getlong(av);
//                break;
//        }
//    }
//
//    x->myVOMM->SetInitialProb(&data[0], ac);
//
//    delete[] data;
//}
//void ml_vomm_dump(t_ml_vomm* x) {
//    ml_vomm_getTransitions(x);
//    ml_vomm_getEmissions(x);
//    ml_vomm_getInitial(x);
//    ml_vomm_getStates(x);
//    ml_vomm_getAlphabet(x);
//}
//void ml_vomm_getEmissions(t_ml_vomm* x) {
//    x->myVOMM->GetEmissions(x->out2);
//}
//void ml_vomm_getTransitions(t_ml_vomm* x) {
//    x->myVOMM->GetTransitions(x->out2);
//}
//void ml_vomm_getEmission(t_ml_vomm* x, long column) {
//    x->myVOMM->GetEmissions(x->out2, column);
//}
//void ml_vomm_getTransition(t_ml_vomm* x, long row) {
//    x->myVOMM->GetTransitions(x->out2, row);
//}
//void ml_vomm_getInitial(t_ml_vomm* x) {
//    x->myVOMM->GetInitialProb(x->out2);
//}

//void ml_vomm_query(t_ml_vomm *x, t_symbol *s, short ac, t_atom *av) {
//    vector<double> data;
//    
//    for (int i = 0; i < ac; i++, av++) {
//        int t = atom_gettype(av);
//        switch (t) {
//            case A_FLOAT:
//                data.push_back(atom_getfloat(av));
//                break;
//            case A_LONG:
//                data.push_back((double)atom_getlong(av));
//                break;
//        }
//    }
//    
//    vector<double>* weights = x->myVOMM->Query(&data);
//    if (weights == NULL)
//        return;
//    
//    t_atom *out = new t_atom[weights->size()+1];
//    atom_setsym(out, gensym("weights"));
//    int i = 1;
//    for (vector<double>::iterator it = weights->begin(); it != weights->end(); it++, i++) {
//        atom_setfloat(&out[i], *it);
//    }
//
//    outlet_atoms(x->outlet, (short)weights->size()+1, out);
//	delete[] out;
//}
//void ml_vomm_random(t_ml_vomm* x) {
//    // randomize the membership weights
//    x->myVOMM->randomize();
//}
