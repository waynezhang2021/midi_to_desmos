#include<fstream>
#include<sstream>
#include<vector>
#include<cmath>
#include<cstdlib>
#include<cstdio>
using namespace std;
struct note
{
	double freq;
	double begin;
	double end;
};
double process_note(string note)
{
	static int tbl[]={0,2,4,5,7,9,11};
	double base=261.6255653005986346778499935234/16;
	double ratio=1.0594630943592952645618252949463;
	if(note[0]>='C'&&note[0]<='G')
		base*=pow(ratio,tbl[note[0]-'C']);
	else
		base*=pow(ratio,tbl[((note[0]=='A')?5:6)]);
	if(note[1]=='#')
	{
		base*=ratio;
		base*=pow(2,atoi(note.substr(2).c_str()));
		return base;
	}
	else
	{
		base*=pow(2,atoi(note.substr(1).c_str()));
		return base;
	}
}
string dts(double n)
{
	string t=to_string(n);
	if(n!=0)
		t=t.substr(0,t.find('.')-1);
	else
		t="0";
	return t;
}
int main(int argc,char** argv)
{
	if(argc<3)
	{
		printf("usage:%s <input_file> <quantization_step>\n",argv[0]);
		return 1;
	}
	string filename=argv[1];
	double quantity=stod(argv[2]);
	ifstream fin(filename);
	vector<note> notes;
	string line;
	getline(fin,line);
	double begin,end,duration,velocity;
	string note_name;
	while(fin>>note_name>>begin>>end>>duration>>velocity)
		notes.push_back({process_note(note_name),begin,end});
	vector<vector<note>> tracks;
	vector<double> end_time;
	int target;
	double min_end;
	for(note n:notes)
	{
		target=-1;
		min_end=1e300;
		for(int i=0;i<end_time.size();i++)
			if(end_time[i]<=n.begin&&end_time[i]<min_end)
			{
				min_end=end_time[i];
				target=i;
			}
		if(target==-1)
		{
			tracks.push_back({n});
			end_time.push_back(n.end);
		}
		else
		{
			tracks[target].push_back(n);
			end_time[target]=n.end;
		}
	}
	double max_end=0;
	for(note n:notes)
		if(n.end>max_end)
			max_end=n.end;
	int num_points=int(ceil(max_end/quantity));
	int start_t,end_t;
	vector<double> stream(num_points,0.0);
	for(int i=0;i<tracks.size();i++)
	{
		for(note n:tracks[i])
		{
			start_t=int(ceil(n.begin/quantity));
			end_t=int((n.end-1e-9)/quantity);
			for(int j=start_t;j<=end_t;j++)
				stream[j]=n.freq;
		}
		printf("l_%d=[",i+1);
		for(int j=0;j<stream.size()-1;j++)
			printf("%d,",int(round(stream[j])));
		printf("%d]\n",int(round(stream[stream.size()-1])));
	}
	return 0;
}
