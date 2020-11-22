#include "GSound.h"

GSound::GSound()
{
	g_pDS = NULL;
}

GSound::~GSound()
{
}

void GSound::LoadSound(HWND _hWnd)
{
	if (!InitDirectSound(_hWnd))
	{
		MessageBox(NULL, "Unable to init DirectSound", "ERROR", MB_OK);
	}
	DSBuffer[INTRODUCE1] = LoadWaveToSoundBuffer("sounds\\background\\Introduce1.wav");
	DSBuffer[INTRODUCE2] = LoadWaveToSoundBuffer("sounds\\background\\Introduce2.wav");
	DSBuffer[LVL1_VAMKILL]
		= LoadWaveToSoundBuffer("sounds\\background\\Level1_VampireKiller.wav");
	DSBuffer[LVL2_STALKER]
		= LoadWaveToSoundBuffer("sounds\\background\\Level2_Stalker.wav");
	DSBuffer[BOOMERANGS] = LoadWaveToSoundBuffer("sounds\\sfx\\BOOMERANG.wav");
	DSBuffer[BRICKBREAK] = LoadWaveToSoundBuffer("sounds\\sfx\\BRICKBREAK.wav");
	DSBuffer[DOWNWATER] = LoadWaveToSoundBuffer("sounds\\sfx\\DOWNWATER.wav");
	DSBuffer[EATCROSS] = LoadWaveToSoundBuffer("sounds\\sfx\\EATCROSS.wav");
	DSBuffer[EATCRYSTAL] = LoadWaveToSoundBuffer("sounds\\sfx\\EATCRYSTAL.wav");
	DSBuffer[EATHEART] = LoadWaveToSoundBuffer("sounds\\sfx\\EATHEART.wav");
	DSBuffer[EATMONEY] = LoadWaveToSoundBuffer("sounds\\sfx\\EATMONEY.wav");
	DSBuffer[EATPOTION] = LoadWaveToSoundBuffer("sounds\\sfx\\EATPOTION.wav");
	DSBuffer[EATWEAPON_CHICKEN]
		= LoadWaveToSoundBuffer("sounds\\sfx\\EATWEAPON_CHICKEN_MULTIPLY.wav");
	DSBuffer[HITSOFT] = LoadWaveToSoundBuffer("sounds\\sfx\\HITSOFT.wav");
	DSBuffer[HITSTEEL] = LoadWaveToSoundBuffer("sounds\\sfx\\HITSTEEL.wav");
	DSBuffer[HOLYWATERBREAK] = LoadWaveToSoundBuffer("sounds\\sfx\\HOLYWATERBREAK.wav");
	DSBuffer[INJURED] = LoadWaveToSoundBuffer("sounds\\sfx\\INJURED.wav");
	DSBuffer[SOUNDKNIFE] = LoadWaveToSoundBuffer("sounds\\sfx\\KNIFE.wav");
	DSBuffer[MAINGATE] = LoadWaveToSoundBuffer("sounds\\sfx\\MAINGATE.wav");
	DSBuffer[PAUSE] = LoadWaveToSoundBuffer("sounds\\sfx\\PAUSE.wav");
	DSBuffer[POTIONIMMORTAL] = LoadWaveToSoundBuffer("sounds\\sfx\\POTIONIMMORTAL.wav");
	DSBuffer[SHOWSECRETITEM] = LoadWaveToSoundBuffer("sounds\\sfx\\SHOWSECRETITEM.wav");
	DSBuffer[TETRENCAOXUONG] = LoadWaveToSoundBuffer("sounds\\sfx\\TETRENCAOXUONG.wav");
	DSBuffer[TICKTICKTICK] = LoadWaveToSoundBuffer("sounds\\sfx\\TICKTICKTICK.wav");
	DSBuffer[TIMENEAREND] = LoadWaveToSoundBuffer("sounds\\sfx\\TIMENEAREND.wav");
	DSBuffer[UPWATER] = LoadWaveToSoundBuffer("sounds\\sfx\\UPWATER.wav");
	DSBuffer[WHIPATTACK_AXE] = LoadWaveToSoundBuffer("sounds\\sfx\\WHIPATTACK_AXE.wav");
	DSBuffer[BOSS_FIGHT] = LoadWaveToSoundBuffer("sounds\\background\\bossbatle_poisonmind.wav");

	if (!DSBuffer)
	{
		MessageBox(NULL, "Unable to load sound", "ERROR", MB_OK);
	}
}

bool GSound::InitDirectSound(HWND hwnd)
{
	HRESULT hr;

	hr = DirectSoundCreate8(NULL, &g_pDS, NULL);
	if FAILED(hr)
		return false;

	// Set DirectSound cooperative level 
	hr = g_pDS->SetCooperativeLevel(hwnd, DSSCL_PRIORITY);
	if FAILED(hr)
		return false;

	return true;
}

void GSound::shutdownDirectSound()
{
	if (DSBuffer)
	{

		for (int i = 0; i < 30; i++)
		{
			if (DSBuffer[i] != NULL)
			{
				DSBuffer[i]->Release();
				DSBuffer[i] = NULL;
			}
		}
		//delete DSBuffer;
	}
	if (g_pDS)
	{
		g_pDS->Release();
		g_pDS = NULL;
	}
}

LPDIRECTSOUNDBUFFER GSound::LoadWaveToSoundBuffer(char* filename)
{
	LPDIRECTSOUNDBUFFER apDSBuffer = NULL;
	CWaveFile* wavFile;
	HRESULT hr;

	wavFile = new CWaveFile();
	wavFile->Open(filename, NULL, WAVEFILE_READ);


	DSBUFFERDESC dsbd;
	ZeroMemory(&dsbd, sizeof(DSBUFFERDESC));
	dsbd.dwSize = sizeof(DSBUFFERDESC);
	dsbd.dwFlags = 0;
	dsbd.dwBufferBytes = wavFile->GetSize();
	dsbd.guid3DAlgorithm = GUID_NULL;
	dsbd.lpwfxFormat = wavFile->m_pwfx;

	hr = g_pDS->CreateSoundBuffer(&dsbd, &apDSBuffer, NULL);
	if FAILED(hr)
	{
		MessageBox(NULL, "unable to create sound buffer", "ERROR", MB_OK);
		return NULL;
	}

	VOID* pDSLockedBuffer = NULL; // Pointer to locked buffer memory
	DWORD   dwDSLockedBufferSize = 0;    // Size of the locked DirectSound buffer
	DWORD   dwWavDataRead = 0;    // Amount of data read from the wav file 

	hr = apDSBuffer->Lock(0, wavFile->GetSize(),
		&pDSLockedBuffer, &dwDSLockedBufferSize,
		NULL, NULL, 0L);
	if FAILED(hr)
		return NULL;

	// Reset the wave file to the beginning 
	wavFile->ResetFile();

	// Read the wave file
	hr = wavFile->Read((BYTE*)pDSLockedBuffer,
		dwDSLockedBufferSize,
		&dwWavDataRead);
	// Check to make sure that this was successful
	if FAILED(hr)
		return NULL;

	// Check to make sure the wav file is not empty
	if (dwWavDataRead == 0)
	{
		// Wav is blank, so just fill with silence
		FillMemory((BYTE*)pDSLockedBuffer,
			dwDSLockedBufferSize,
			(BYTE)(wavFile->m_pwfx->wBitsPerSample == 8 ? 128 : 0));
	}
	else if (dwWavDataRead < dwDSLockedBufferSize)
	{
		// Don't repeat the wav file, just fill in silence 
		FillMemory((BYTE*)pDSLockedBuffer + dwWavDataRead,
			dwDSLockedBufferSize - dwWavDataRead,
			(BYTE)(wavFile->m_pwfx->wBitsPerSample == 8 ? 128 : 0));
	}

	// Unlock the buffer, we don't need it anymore.
	apDSBuffer->Unlock(pDSLockedBuffer, dwDSLockedBufferSize, NULL, 0);

	// Clean up
	delete wavFile;

	return apDSBuffer;
}

void GSound::playSound(int ID)
{
	DSBuffer[ID]->Play(0, 0, 0);
}

void GSound::playSoundLoop(int ID)
{
	DSBuffer[ID]->Play(0, 0, DSBPLAY_LOOPING);
}

void GSound::stopSound(int ID)
{
	DSBuffer[ID]->Stop();
}
