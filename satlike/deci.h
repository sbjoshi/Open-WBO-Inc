#ifndef _DECI_H_
#define _DECI_H_

#include "basis_pms.h"

using namespace std;

class Decimation
{
    public:
    Decimation(lit** ls_var_lit, int* ls_var_lit_count, lit** ls_clause_lit, long long* ls_org_clause_weight, long long ls_top_clause_weight);

    void make_space(int max_c, int max_v);
    void free_memory();
    void init(int* ls_local_opt, int* ls_global_opt, lit* ls_unit_clause, int ls_unit_clause_count, int* ls_clause_lit_count);
    void push_unit_clause_to_queue(lit tem_l);
    void assign(int v, int sense);
    void remove_unassigned_var(int v);
    void hunit_propagation();
    void sunit_propagation();
    void random_propagation();
    void unit_prosess();
    bool choose_sense(int v);

    int* fix;
    
    int num_vars;
    int num_clauses;

    lit** clause_lit;
    lit** var_lit;
    int* var_lit_count;

    int* local_opt;
    int* global_opt;
    long long* org_clause_weight;
    long long top_clause_weight;

    lit* hunit_clause_queue;
    int* sense_in_hunit_clause_queue;
    int hunit_beg_pointer;
    int hunit_end_pointer;

    lit* sunit_clause_queue;
    int* sense_in_sunit_clause_queue;
    int sunit_beg_pointer;
    int sunit_end_pointer;

    int* unassigned_var;
    int* index_in_unassigned_var;
    int unassigned_var_count;

    int* clause_delete;
    int* clause_lit_count;
};

Decimation::Decimation(lit** ls_var_lit, int* ls_var_lit_count, lit** ls_clause_lit, long long* ls_org_clause_weight, long long ls_top_clause_weight)
{
    var_lit=ls_var_lit;
    var_lit_count=ls_var_lit_count;
    clause_lit=ls_clause_lit;
    org_clause_weight=ls_org_clause_weight;
    top_clause_weight=ls_top_clause_weight;
}

void Decimation::make_space(int max_c, int max_v)
{
    num_vars=max_v;
    num_clauses=max_c;

    max_c+=10;
    max_v+=10;

    fix=new int[max_v];

    hunit_clause_queue=new lit[max_v];
    sense_in_hunit_clause_queue=new int[max_v];

    sunit_clause_queue=new lit[max_v];
    sense_in_sunit_clause_queue=new int[max_v];

    unassigned_var=new int[max_v];
    index_in_unassigned_var=new int[max_v];

    clause_delete=new int[max_c];
    clause_lit_count=new int[max_c];
}

void Decimation::free_memory()
{
    delete[] fix;

    delete[] hunit_clause_queue;
    delete[] sense_in_hunit_clause_queue;

    delete[] sunit_clause_queue;
    delete[] sense_in_sunit_clause_queue;

    delete[] unassigned_var;
    delete[] index_in_unassigned_var;

    delete[] clause_delete;
    delete[] clause_lit_count;
}

void Decimation::init(int* ls_local_opt, int* ls_global_opt, lit* ls_unit_clause, int ls_unit_clause_count, int* ls_clause_lit_count)
{
    int v;
    //parameters used in decimation
    hunit_beg_pointer=0;
    hunit_end_pointer=0;

    sunit_beg_pointer=0;
    sunit_end_pointer=0;

    unassigned_var_count=num_vars;

    //data structure of the instance
    local_opt=ls_local_opt;
    global_opt=ls_global_opt;

    for(int i=0;i<num_vars;++i)
    {
        v=i+1;
        unassigned_var[i]=v;
        index_in_unassigned_var[v]=i;

        fix[v]=-1;
        sense_in_hunit_clause_queue[v]=-1;// not in hunit queue
        sense_in_sunit_clause_queue[v]=-1;// not in sunit queue
    }

    for(int i=0;i<num_clauses;++i)
    {
        clause_lit_count[i]=ls_clause_lit_count[i];
        clause_delete[i]=0;
    }

    for(int i=0;i<ls_unit_clause_count;++i)
    {
        push_unit_clause_to_queue(ls_unit_clause[i]);
    }
}

void Decimation::push_unit_clause_to_queue(lit tem_l)
{
    int v=tem_l.var_num;
    int c=tem_l.clause_num;
    if(org_clause_weight[c]==top_clause_weight)
    {
        if(sense_in_hunit_clause_queue[v]==-1)
        {
            sense_in_hunit_clause_queue[v]=tem_l.sense;
            hunit_clause_queue[hunit_end_pointer++]=tem_l;
        }
        else
        {
            if(sense_in_hunit_clause_queue[v]!=tem_l.sense)//conflict var in hard unit queue
            {
                sense_in_hunit_clause_queue[v]=-2;//means this variable is conflict in hard unit queue
            }
        }
    }
    else
    {
        if(sense_in_hunit_clause_queue[v]!=-1) return ;//be defined by hard unit queue

        if(sense_in_sunit_clause_queue[v]==-1)
        {
            sense_in_sunit_clause_queue[v]=tem_l.sense;
            sunit_clause_queue[sunit_end_pointer++]=tem_l;
        }
        else
        {
            if(sense_in_sunit_clause_queue[v]!=tem_l.sense)//conflict var in hard unit queue
            {
                sense_in_sunit_clause_queue[v]=-3;//means this variable is conflict in hard unit queue
            }
        }
    }
}

void Decimation::remove_unassigned_var(int v)
{
    int index=index_in_unassigned_var[v];
    int last_var=unassigned_var[--unassigned_var_count];
    unassigned_var[index]=last_var;
    index_in_unassigned_var[last_var]=index;
}

void Decimation::assign(int v, int sense)
{
    int c,l;

    fix[v]=sense;
    remove_unassigned_var(v);

    for(int i=0;i<var_lit_count[v];++i)
    {
        c=var_lit[v][i].clause_num;
        if(clause_delete[c]==1) continue;

        if(sense==var_lit[v][i].sense)
        {
            clause_delete[c]=1;
            continue;
        }
        for(int j=0;j<clause_lit_count[c];j++)
        {
            if(clause_lit[c][j].var_num==v)
            {
                swap(clause_lit[c][j],clause_lit[c][--clause_lit_count[c]]);
                break;
            }
        }
        if(clause_lit_count[c]==1)
        {
            push_unit_clause_to_queue(clause_lit[c][0]);
        }
    }
}

bool Decimation::choose_sense(int v)
{
    return local_opt[v];
}

void Decimation::hunit_propagation()
{
    int v,c,sense;
    v=hunit_clause_queue[hunit_beg_pointer].var_num;
    sense=hunit_clause_queue[hunit_beg_pointer].sense;
    hunit_beg_pointer++;

    if(sense_in_hunit_clause_queue[v]==-2)
    {
        sense=choose_sense(v);
    }
    assign(v,sense);
}

void Decimation::sunit_propagation()
{
    int v,c,sense;
    v=sunit_clause_queue[sunit_beg_pointer].var_num;
    sense=sunit_clause_queue[sunit_beg_pointer].sense;
    sunit_beg_pointer++;

    if(fix[v]!=-1) return ;

    if(sense_in_sunit_clause_queue[v]==-3)
    {
        sense=choose_sense(v);
    }
    assign(v,sense);
}

void Decimation::random_propagation()
{
    int v,sense;
    v=unassigned_var[rand()%unassigned_var_count];
    sense=local_opt[v];
    assign(v,sense);
}

void Decimation::unit_prosess()
{
    while(unassigned_var_count>0)
    {
        if(hunit_beg_pointer!=hunit_end_pointer)
        {
            hunit_propagation();
        }
        else if(sunit_beg_pointer!=sunit_end_pointer)
        {
            sunit_propagation();
        }
        else
        {
            random_propagation();
        }
    }
}

#endif
