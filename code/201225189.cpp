#include<bits/stdc++.h>
using namespace std;
typedef bool (*comp)(pair<vector<string>,int>,pair<vector<string>,int>);
vector<int>sort_order;
bool compare_q(pair<vector<string>,int> a,pair<vector<string>,int> b)
{
    for(int i=0;i<sort_order.size();i++)
    {
        if(a.first[sort_order[i]] < b.first[sort_order[i]])
            return true;
        else if(a.first[sort_order[i]] > b.first[sort_order[i]])
            return false;
    }
    if(a.second < b.second)
        return true;
    else
        return false;
}
int asc_order = -1;
vector<int> curr_line(100000,0);
//priority_queue<pair<vector<string>,int>, vector<pair<vector<string>,int> >,greater<pair<vector<string>,int> > > pq;
priority_queue<pair<vector<string>,int>, vector<pair<vector<string>,int> >, comp> pq(compare_q);
priority_queue<pair<vector<string>,int>, vector<pair<vector<string>,int> > > pq1;
vector<string> split_string(string line, char delim)
{
    vector<string> ans;
    string curr;
    for(int i=0;i<line.size();i++)
    {
        if(line[i]==delim)
        {
            ans.push_back(curr);
            curr.clear();
        }
        else
        {
            curr.push_back(line[i]);
        }
    }
    if(curr.size() > 0)
    {
        ans.push_back(curr);
    }
    return ans;
}



vector<int> store_sublists(char *inp_file,int num_sublists,int lines_sublist)
{
    vector<int>num_lines_sublist;
    num_lines_sublist.push_back(0);
    ifstream file(inp_file);
    string line;
    int num_lines = 0;
    int sublist_no = 1;
    ofstream out_file;
    string file_name;
    out_file.open("sublist1.txt");
    int flag = -1;
    while(getline(file,line))
    {
        num_lines++;
        if(num_lines <= lines_sublist)
        {
            out_file << line << endl;
            flag = 1;
        }
        else
        {
            flag = 0;
            out_file.close();
            num_lines_sublist.push_back(num_lines-1);
            sublist_no++;
            num_lines = 1;
            stringstream s;
            s << sublist_no;
            file_name ="sublist" + s.str() + ".txt"; 
            out_file.open(file_name.c_str());
            out_file << line << endl;
        }
    }
    if(flag==1)
    {
        num_lines_sublist.push_back(num_lines);
    }
    return num_lines_sublist;
}
bool compare_func(vector<string> l1, vector<string> l2)
{
    for(int i=0;i<sort_order.size();i++)
    {
        if(asc_order==1)
        {
            if(l1[sort_order[i]] > l2[sort_order[i]])
                return false;
            else if(l1[sort_order[i]] < l2[sort_order[i]])
                return true;
        }
        else if(asc_order==0)
        {
            if(l1[sort_order[i]] < l2[sort_order[i]])
                return false;
            else if(l1[sort_order[i]] > l2[sort_order[i]])
                return true;
        }
    }
    return false;
}
void sort_sublists(int num_sublists)
{
    for(int i=1;i<=num_sublists;i++)
    {
        stringstream s;
        s << i;
        string file_name = "sublist" + s.str() + ".txt";
        ifstream curr_file(file_name.c_str());
        vector<vector<string> > lines;
        string line;
        while(getline(curr_file,line))
        {
            vector<string> curr_line = split_string(line,' ');
            lines.push_back(curr_line);
        }
        curr_file.close();
        stable_sort(lines.begin(),lines.end(),compare_func);
        cout << endl;

        ofstream out_file;
        out_file.open(file_name.c_str());
        for(int j=0;j<lines.size();j++)
        {
            for(int k=0;k<lines[j].size();k++)
            {
                out_file << lines[j][k];
                if(k!=(lines[j].size()-1))
                {
                    out_file << " ";
                }
            }
            out_file << endl;
        }
        out_file.close();
    }
}

void pushLine(int blocknum, int num,vector<int> num_lines_sublist,int line_size)
{
        int i;
        if(num_lines_sublist[blocknum] < num)
        {
                cout << "return" << endl;
                return;
        }
        string line;
        stringstream s;
        s << blocknum;
        string file_name = "sublist" + s.str() + ".txt";
        fstream file(file_name.c_str());
        file.seekg((num-1)*(line_size+1));
        getline(file, line);
        file.close();
        vector<string> tmp = split_string(line, ' ');
        if(asc_order == 1)
        {
                pq.push(make_pair(tmp, blocknum));
        }
        else if(asc_order == 0)
        {
                pq1.push(make_pair(tmp, blocknum));
        }
}

void merge_sublists(int num_sublists,char *out_file,vector<int> num_lines_sublist,int line_size)
{
        vector<string> tmp;
        string line;
        int i;
        for(i=1;i<=num_sublists;i++)
        {
                curr_line[i]++;
                pushLine(i, curr_line[i],num_lines_sublist,line_size);
        }
        cout << "pq created" << endl;
        ofstream output(out_file);
        if(asc_order==1)
        {
            while(!pq.empty())
            {
                pair<vector<string>, int>p = pq.top();
                pq.pop();
                tmp = p.first;
                for(i=0; i<tmp.size(); i++)
                {
                    output << tmp[i];
                    if(i != tmp.size()-1)
                        output << " ";
                }
                output << endl;
                curr_line[p.second]++;
                pushLine(p.second, curr_line[p.second],num_lines_sublist,line_size);
            }
        }
        else if(asc_order==0)
        {
            while(!pq1.empty())
            {
                pair<vector<string>, int>p = pq1.top();
                pq1.pop();
                tmp = p.first;
                for(i=0; i<tmp.size(); i++)
                {
                    output << tmp[i];
                    if(i != tmp.size()-1)
                        output << " ";
                }
                output << endl;
                curr_line[p.second]++;
                pushLine(p.second, curr_line[p.second],num_lines_sublist,line_size);
            }
        }
        cout << "done" << endl;
}


int main(int argc,char *argv[])
{
    clock_t begin = clock();
    if(argc < 4)
    {
        cout << "Less number of arguments" << endl;
        return 0;
    }
    if(strcmp(argv[4],"asc")==0)
    {
        asc_order = 1;
    }
    else if(strcmp(argv[4],"desc")==0)
        asc_order = 0;
    else
    {
        cout << "invalid argument 4" << endl;
        return 0;
    }

    vector<int> sublist_line(100000, 0);
    int mem_size = atoi(argv[3]);
    char *inp_file = argv[1];
    char *out_file = argv[2];
    map<string,int> column_size;//mapping column name to its size
    map<string,int> column_no;//mapping column name to its number in the file
    ifstream meta_file("metadata.txt");
    string line;
    int line_size;
    int col_no = 0;
    if(meta_file)
    {
        while(getline(meta_file,line))
        {

            vector<string> temp = split_string(line,',');
            column_size.insert(make_pair(temp[0],atoi(temp[1].c_str()) ) );
            column_no.insert(make_pair(temp[0],col_no));
            col_no++;
        }

    }
    else
    {
        cout << "No metadata file" << endl;
        return 0;
    }
    map<int,int>col_given;
    for(int i=5;i<argc;i++)
    {
        string col_name = argv[i];
        sort_order.push_back(column_no[col_name]);
        col_given.insert(make_pair(column_no[col_name],1));
    }
    
    /*for(int i=0;i<column_no.size();i++)
    {
        if(col_given.count(i) <= 0)
        {
            sort_order.push_back(i);
        }

    }*/
    int total_lines = 0;
    ifstream file(inp_file);
    while(getline(file,line))
    {
        line_size = line.size();
        total_lines++;
    }
    int total_size = total_lines*line_size;
    int num_sublists = (total_lines*line_size)/(mem_size*1024*1024);
    if(total_size%(mem_size*1024*1024)!=0)
        num_sublists++;
    int lines_sublist = total_lines/num_sublists;
    cout << "Number of columns : " << col_no << endl;
    cout << "Number of lines in input file : " << total_lines << endl;
    cout << "Size in bytes of each line in the input file : " << line_size << endl;
    cout << "Total size in bytes of the input_file : " << total_size << endl; 
    cout << "Total size in bytes of the main memory : " << mem_size*1024*1024 << endl; 
    cout << "Number of Sublists : " << num_sublists << endl;
    cout << "Number of lines in each sublist : " << lines_sublist << endl;
    vector<int> num_lines_sublist = store_sublists(inp_file,num_sublists,lines_sublist);
    sort_sublists(num_sublists);
    merge_sublists(num_sublists,out_file,num_lines_sublist,line_size);
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    cout << elapsed_secs << endl;
    return 0;
}
