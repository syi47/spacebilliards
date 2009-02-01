#pragma once



class GameTimer
{
public:
	static GameTimer& getInstance()
	{
		static GameTimer m_Timer;
		return m_Timer;
	}

	void start();

	void stop();

	int getTimeElapsedInMilliseconds();

private:
	GameTimer(void);
	~GameTimer(void);
};
