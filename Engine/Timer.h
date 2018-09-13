#pragma once

class Timer
{
public:
	Timer() = delete;
	Timer( float time )
		:
		maxTime( time ),
		curTime( 0.0f )
	{}
	void Update( float dt )
	{
		if( curTime <= maxTime ) curTime += dt;
	}
	void Reset()
	{
		curTime = 0.0f;
	}
	void Reset( float newMaxTime )
	{
		curTime = 0.0f;
		maxTime = newMaxTime;
	}
	bool IsDone() const
	{
		return( curTime >= maxTime );
	}
	int GetPercent() const
	{
		return( int( ( curTime / maxTime ) * 100.0f ) );
	}
private:
	float curTime;
	float maxTime;
};