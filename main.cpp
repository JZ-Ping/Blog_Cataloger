// Project UID db1f506d06d84ab787baf250c265e24e
#include <iostream>
#include <cassert>
#include <string>
#include "csvstream.h"
#include <map>
#include <set>
#include <stdio.h>
#include <cmath>
using namespace std;

class Classifier
{
private:
    int post_num;
    int uniword_num;
    //set<string> word_set;
    map<string, int> label2posts;
    map<string, int> word2occr;
    map<pair<string, string>, int> wordinlabel;



    void print_debug(){
        cout<<"vocabulary size = "<<uniword_num<<endl<<endl;
        cout<<"classes:"<<endl;
        for(auto it = label2posts.begin(); it!=label2posts.end();
        ++it){
            cout<<"  "<<it->first<<", "<<it->second
                <<" examples, log-prior = "
                <<log_prior(it->first)<<endl;
        }
        cout<<"classifier parameters:"<<endl;
        for(auto it = wordinlabel.begin(); it!=wordinlabel.end();
        ++it){
            cout<<"  "<<it->first.first<<":"<<it->first.second
            <<", count = "<<it->second<<", log-likelihood = "
            <<log_likelihood(it->first.first, it->first.second)<<endl;
        }
        cout<<endl;

    }

    double log_prior(string label){
        double ratio = double(label2posts[label])/post_num;
        return log(ratio);
    }

    double log_likelihood(string label, string word){
        map<pair<string, string>, int>::iterator it = wordinlabel.find({label, word});
        if(it!=wordinlabel.end()){
            double ratio = double(wordinlabel[{label, word}])/label2posts[label];
            return log(ratio);
        } 
        map<string, int>::iterator it2 = word2occr.find(word);
        if(it2!=word2occr.end()){
            double ratio = double(word2occr[word])/post_num;
            return log(ratio);
        }
        return log(1/double(post_num));
    }
    
    string unique_words(istringstream &line) {
        set<string> words;
        string word;
        while (line >> word) {
        words.insert(word);
        }
        string uniqline;
        for(set<string>::iterator i = words.begin();
            i != words.end(); i++){
                uniqline = uniqline+" "+*i;
            }
        return uniqline;
    }

    pair<string, double> predict(string &uniq_line){
        map<string, double> prob_list;
        
        //cout<<uniq_line<<endl;
        string word;
        // Traverse through labels
        for(map<string, int>::iterator itr = label2posts.begin(); 
            itr != label2posts.end(); itr++){
                double prob = log_prior(itr->first);
                istringstream uniq(uniq_line);
                while(uniq>>word){
                    prob+=log_likelihood(itr->first, word);
                }
                prob_list[itr->first] = prob;
                //cout<<itr->first<<" "<<prob<<endl;
            }
        pair<string, double> max_prob_label(prob_list.begin()->first,
            prob_list.begin()->second);
        for(map<string, double>::iterator itr = prob_list.begin(); 
            itr != prob_list.end(); itr++){
                if(itr->second > max_prob_label.second){
                    max_prob_label.first = itr->first;
                    max_prob_label.second = itr->second;
                }
            }
        return max_prob_label;
    }

public:

    Classifier()
        :post_num(0), uniword_num(0){}

    void read_file(csvstream &trainfile, bool debug){
        map<string, string> row;
        if(debug) cout<<"training data:"<<endl;
        while (trainfile >> row)
        {
            string label = row["tag"];
            label2posts[label] ++;
            post_num++;
            istringstream cont(row["content"]);
            string word;
            // Print the training data if debugging
            if(debug) {
                cout<<"  label = "<<label
                <<", content = "<< row["content"]<<endl;
            }
            // Create a set of unique words from line
            istringstream cont_uniq(unique_words(cont));
            while(cont_uniq >> word){
                word2occr[word]++;
                wordinlabel[{label, word}]++;
            }
        }
        cout.precision(3);
        uniword_num = word2occr.size();
        cout<<"trained on "<<post_num<<" examples"<<endl;
        if(!debug) cout<<endl;
        if(debug) print_debug();
    };

    void test_file(csvstream &testfile){
        map<string, string> row;
        cout<<"test data:"<<endl;
        cout.precision(3);
        int post_num = 0;
        int corr_post_num = 0;
        while (testfile >> row)
        {   
            post_num++;
            string label = row["tag"];
            istringstream cont(row["content"]);
            string uniq_cont = unique_words(cont);
            pair<string, double> pred_label = predict(uniq_cont);
            if(pred_label.first==label) corr_post_num++;
            cout<<"  correct = "<<label<<", predicted = "<<pred_label.first<<
                ", log-probability score = "<<pred_label.second<<endl;
            cout<<"  content = "<<row["content"]<<endl<<endl;
        }
        cout<< "performance: "<< corr_post_num <<" / "<<post_num<<
            " posts predicted correctly"<<endl;
    };
};


int main(int argc, char *argv[]) {
    // Check Command Arguments
    if(argc!=3&&argc!=4){
        cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << endl;
        return -1;
    }
    if(argc==4 && strcmp(argv[3], "--debug")){
        cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << endl;
        return -1;
    }
//    const char* fname = argv[1];
//    FILE* trf = fopen(fname, 'r');
//    if(trf==NULL)

    // Open files
    csvstream trainfile(argv[1]);
    csvstream testfile(argv[2]);
    // How to check if file is open?
    
    Classifier mycls;
    bool debug = false;
    if(argc==4) debug = true;
    mycls.read_file(trainfile, debug);
    mycls.test_file(testfile);
    return 0;
}
