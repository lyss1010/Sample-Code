#include "Channel.h"
#include <iostream>
#include <math.h>
using namespace std;
Channel::Channel()
{
}

Channel::~Channel()
{
}
void Channel::Precompute(){
	beginTime = keyframes[0]->Time;
	beginValue = keyframes[0]->Value;
	if (keyframes.size() == 1){												//special case(only one keyframe) 
		lastTime = beginTime;			
		lastValue = beginValue;
		keyframes[0]->TangentIn = 0;
		keyframes[0]->TangentOut = 0;
	}
	else{																	//keyframe more than one
		lastTime = keyframes[keyframes.size() - 1]->Time;
		lastValue = keyframes[keyframes.size() - 1]->Value;
		for (int i = 0; i < keyframes.size(); i++){
			float p0, p1, v0, v1, t0, t1;


			//precompute TangentIn
			if (keyframes[i]->RuleIn.compare("flat") == 0){						//flat -> v = 0
				keyframes[i]->TangentIn = 0;
				//cout << "flat" << endl;
			}
			else if (keyframes[i]->RuleIn.compare("linear") == 0){				//linear ->  V0out = V1in =  (p1-p0) / (t1-t0)
				if (i == 0){
					keyframes[i]->TangentIn = 0;
				}
				else
				{
					p0 = keyframes[i - 1]->Value;
					p1 = keyframes[i]->Value;
					t0 = keyframes[i - 1]->Time;
					t1 = keyframes[i]->Time;
					keyframes[i]->TangentIn = (p1 - p0) / (t1 - t0);
				}
				//cout << "linear" << endl;										//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!CHECK THIS LATER!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			}
			else if (keyframes[i]->RuleIn.compare("smooth") == 0){				//smooth -> (automatically adjust tangent for smooth results)
				if (i == 0){
					keyframes[i]->TangentIn = 0;
				}
				else if (i == (keyframes.size() - 1)){
					p0 = keyframes[i - 1]->Value;
					p1 = keyframes[i]->Value;
					t0 = keyframes[i - 1]->Time;
					t1 = keyframes[i]->Time;
					keyframes[i]->TangentIn = (p1 - p0) / (t1 - t0);
				}
				else{
					p0 = keyframes[i - 1]->Value;
					p1 = keyframes[i + 1]->Value;
					t0 = keyframes[i - 1]->Time;
					t1 = keyframes[i + 1]->Time;
					keyframes[i]->TangentIn = (p1 - p0) / (t1 - t0);
				}
				//cout << "smooth" << endl;
			}
			else if (keyframes[i]->RuleIn.compare("fixed") == 0){				//fixed -> user can arbitrarily specify a value
				//cout << "fixed" << endl;
			}

			//precompute TangentOut
			if (keyframes[i]->RuleOut.compare("flat") == 0){					//flat v = 0
				keyframes[i]->TangentOut = 0;
				//cout << "flat" << endl;
			}
			else if (keyframes[i]->RuleOut.compare("linear") == 0){				//linear ->  V0out = V1in =  (p1-p0) / (t1-t0)
				if (i == (keyframes.size() - 1)){
					keyframes[i]->TangentOut = 0;
				}
				else{
					p0 = keyframes[i]->Value;
					p1 = keyframes[i + 1]->Value;
					t0 = keyframes[i]->Time;
					t1 = keyframes[i + 1]->Time;
					keyframes[i]->TangentOut = (p1 - p0) / (t1 - t0);
				}
				//cout << "linear" << endl;
			}
			else if (keyframes[i]->RuleOut.compare("smooth") == 0){				//smooth -> (automatically adjust tangent for smooth results)
				if (i == 0){
					p0 = keyframes[i]->Value;
					p1 = keyframes[i + 1]->Value;
					t0 = keyframes[i]->Time;
					t1 = keyframes[i + 1]->Time;
					keyframes[i]->TangentOut = (p1 - p0) / (t1 - t0);
				}
				else if(i == (keyframes.size()-1)){
					keyframes[i]->TangentOut = 0;
				}
				else{
					p0 = keyframes[i - 1]->Value;
					p1 = keyframes[i + 1]->Value;
					t0 = keyframes[i - 1]->Time;
					t1 = keyframes[i + 1]->Time;
					keyframes[i]->TangentOut = (p1 - p0) / (t1 - t0);
				}
				//cout << "smooth" << endl;
			}
			else if (keyframes[i]->RuleOut.compare("fixed") == 0){				//fixed - > user can arbitrarily specify a value
				//cout << "fixed" << endl;
			}
		}


		for (int i = 0; i < (keyframes.size() - 1); i++){
			float p0, p1, v0, v1, t0, t1;
			p0 = keyframes[i]->Value;
			p1 = keyframes[i + 1]->Value;
			t0 = keyframes[i]->Time;
			t1 = keyframes[i + 1]->Time;
			v0 = keyframes[i]->TangentOut;
			v1 = keyframes[i + 1]->TangentIn;
			keyframes[i]->A = 2 * p0 + (-2)*p1 + (t1 - t0)*v0 + (t1 - t0)*v1;
			keyframes[i]->B = -3 * p0 + 3 * p1 + (-2)*(t1 - t0)*v0 - (t1 - t0)*v1;
			keyframes[i]->C = (t1 - t0)*v0;
			keyframes[i]->D = p0;
		}
	}
}

float Channel::Evaluate(float time){
	/* four cases
	t is before the first key(use extrapolation)
	t is after the last key(use extrapolation)
	t falls exactly on some key(return key value)
	t falls between two keys(evaluate cubic equation)
	*/
	if (keyframes.size() == 1){
		return keyframes[0]->Value;
	}
	if (time < beginTime){			//(use extrapolation)
		//check for first Extrapolation type
		if (ep0.compare("constant") == 0){					//flat return Tmin value
			return keyframes[0]->Value;
		}
		else if (ep0.compare("linear") == 0){
			//cout << "Linear" << endl;
			float slope;
			slope = keyframes[0]->TangentOut;
			if (slope == 0)
				return keyframes[0]->Value;
			return ((time-keyframes[0]->Time)/slope+keyframes[0]->Value);		//Point–slope form ->  returnValue = slope(time-t0)+p0
		}
		else if (ep0.compare("cycle") == 0){		
			//cout << "cycle" << endl;
			float range = lastTime - beginTime;
			if (range <= 0){
				return keyframes[0]->Value;
			}
			float cycleTime;
			cycleTime = fmodf(time-beginTime,range);
			cycleTime += beginTime;
			cycleTime += range;
			for (int i = 0; i < keyframes.size(); i++){
				if (cycleTime == keyframes[i]->Time)
					return keyframes[i]->Value;
			}
			for (int i = 0; i < keyframes.size(); i++){
				if (cycleTime>keyframes[i]->Time&&cycleTime < keyframes[i + 1]->Time){
					float u;
					u = (cycleTime - keyframes[i]->Time) / (keyframes[i + 1]->Time - keyframes[i]->Time);
					float A, B, C, D;
					A = keyframes[i]->A;
					B = keyframes[i]->B;
					C = keyframes[i]->C;
					D = keyframes[i]->D;
					return (D + u*(C + u*(B + u*(A))));
				}
			}
		}
		else if (ep0.compare("cycle_offset") == 0){
		//	cout << "cycle_offset" << endl;
			float range = lastTime - beginTime;
			if (range <= 0){
				return keyframes[0]->Value;
			}
			float cycleTime;
			float cycles;
			cycleTime = fmodf(time - beginTime, range);
			cycleTime += beginTime;
			cycleTime += range;
			for (int i = 0; i < keyframes.size(); i++){
				if (cycleTime == keyframes[i]->Time)
					return keyframes[i]->Value;
			}
			cycles = (time - cycleTime - beginTime) / range;
			for (int i = 0; i < keyframes.size(); i++){
				if (cycleTime>keyframes[i]->Time&&cycleTime < keyframes[i + 1]->Time){
					float u;
					u = (cycleTime - keyframes[i]->Time) / (keyframes[i + 1]->Time - keyframes[i]->Time);
					float A, B, C, D;
					A = keyframes[i]->A;
					B = keyframes[i]->B;
					C = keyframes[i]->C;
					D = keyframes[i]->D;
					float valueRange,offset;
					valueRange = lastValue - beginValue;
					offset = valueRange*cycles;
					return (D + u*(C + u*(B + u*(A))) - offset);
				}
			}
		}
		else if (ep0.compare("bounce") == 0){
			//cout << "Bounce" << endl;
			return 0;
		}
		//return keyframes[0]->Value;
	}
	else if (time > lastTime){		//(use extrapolation)
		//check for second Extrapolation type
		if (ep1.compare("constant") == 0){
			//cout << "constant" << endl;
			return keyframes[keyframes.size()-1]->Value;
		}
		else if (ep1.compare("linear") == 0){
			//cout << "Linear" << endl;
			float slope;
			slope = keyframes[keyframes.size() - 1]->TangentIn;
			if (slope == 0)
				return keyframes[keyframes.size() - 1]->Value;
			return ((time - keyframes[keyframes.size() - 1]->Time) / slope + keyframes[keyframes.size() - 1]->Value);		//Point–slope form ->  returnValue = slope(time-Tmax)+Pmax
		}
		else if (ep1.compare("cycle") == 0){
			//cout << "cycle" << endl;
			float range = lastTime - beginTime;
			if (range == 0){
				return keyframes[0]->Value;
			}
			float cycleTime;
			cycleTime = fmodf(time - beginTime, range);
			cycleTime += beginTime;
			for (int i = 0; i < keyframes.size(); i++){
				if (cycleTime == keyframes[i]->Time)
					return keyframes[i]->Value;
			}
			for (int i = 0; i < keyframes.size()-1; i++){
				if (cycleTime>keyframes[i]->Time&&cycleTime < keyframes[i + 1]->Time){
					float u;
					u = (cycleTime - keyframes[i]->Time) / (keyframes[i + 1]->Time - keyframes[i]->Time);
					float A, B, C, D;
					A = keyframes[i]->A;
					B = keyframes[i]->B;
					C = keyframes[i]->C;
					D = keyframes[i]->D;
					return (D + u*(C + u*(B + u*(A))));
				}
			}
		}
		else if (ep1.compare("cycle_offset") == 0){
			//cout << "cycle_offset" << endl;
			float range = lastTime - beginTime;
			if (range == 0){
				return keyframes[0]->Value;
			}
			float cycleTime;
			float cycles;
			cycleTime = fmodf(time - beginTime, range);
			cycleTime += beginTime;
			for (int i = 0; i < keyframes.size(); i++){
				if (cycleTime == keyframes[i]->Time)
					return keyframes[i]->Value;
			}
			cycles = (time - cycleTime - beginTime) / range;
			for (int i = 0; i < keyframes.size(); i++){
				if (cycleTime>keyframes[i]->Time&&cycleTime < keyframes[i + 1]->Time){
					float u;
					u = (cycleTime - keyframes[i]->Time) / (keyframes[i + 1]->Time - keyframes[i]->Time);
					float A, B, C, D;
					A = keyframes[i]->A;
					B = keyframes[i]->B;
					C = keyframes[i]->C;
					D = keyframes[i]->D;
					float valueRange, offset;
					valueRange = lastValue - beginValue;
					offset = valueRange*cycles;
					return (D + u*(C + u*(B + u*(A))) + offset);
				}
			}
		}
		else if (ep1.compare("bounce") == 0){
			//cout << "Bounce" << endl;
			return 0;
		}
		//return keyframes[0]->Value;
	}
	else{
		for (int i = 0; i < keyframes.size(); i++){
			if (time == keyframes[i]->Time)
				return keyframes[i]->Value;
		}
		//case when between two keys
		//evaluate cubic equation here
		//check between which two keys
		for (int i = 0; i < keyframes.size(); i++){
			if (time>keyframes[i]->Time&&time < keyframes[i + 1]->Time){
				float u;
				u = (time - keyframes[i]->Time) / (keyframes[i + 1]->Time - keyframes[i]->Time);
				float A, B, C, D;
				A = keyframes[i]->A;
				B = keyframes[i]->B;
				C = keyframes[i]->C;
				D = keyframes[i]->D;
				return (D + u*(C + u*(B + u*(A))));
			}
		}
	}
	cout << "caonima" << endl;
}