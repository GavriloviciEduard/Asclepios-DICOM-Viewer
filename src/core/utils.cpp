#include "utils.h"

void asclepios::core::Utils::processTagFormat(const vtkDICOMTag& t_tag, std::string& t_value)
{
	if (t_value.empty())
	{
		return;
	}
	switch (t_tag.GetKey())
	{
	case 524320: //study date
		t_value = getDateFormat(t_value);
		break;
	case 524338: //study time
		t_value = getTimeFormat(t_value);
		break;
	case 1048624: //birth date
		t_value = getDateFormat(t_value);
		break;
	case 1048640: //sex
		t_value = "[" + t_value + "]";
		break;
	case 1052688: //age
		if (t_value.length() < 3)
		{
			t_value = "Age: " + t_value;
			break;
		}
		if (t_value.at(3) == 'Y')
		{
			if (t_value.at(0) == '0')
			{
				t_value = "Age: " + t_value.substr(1, 2);
			}
			else t_value = "Age: " + t_value.substr(0, 3);
		}
		else
		{
			t_value = "Age: " + t_value.substr(1, 2) + " months";
		}
		break;
	default:
		break;
	}
}

//-----------------------------------------------------------------------------
std::string asclepios::core::Utils::getDateFormat(const std::string& t_date)
{
	return t_date.substr(6, 2) + "/" +
		t_date.substr(4, 2) + "/" +
		t_date.substr(0, 4);
}

//-----------------------------------------------------------------------------
std::string asclepios::core::Utils::getTimeFormat(const std::string& t_time)
{
	return t_time.substr(0, 2) + ":" +
		t_time.substr(2, 2) + ":" +
		t_time.substr(4, 2);
}
