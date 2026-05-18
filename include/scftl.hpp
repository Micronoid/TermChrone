// scftl.hpp
#ifndef SCFTL_HPP
#define SCFTL_HPP

#include <string>
#include <vector>
#include <cstdint>
#include <unordered_map>
#include <functional>
#include <cctype>

namespace scftl {

// ======================== decode ========================

static std::vector<uint32_t> utf8ToCodepoints(const std::string& str) {
    std::vector<uint32_t> result;
    for (size_t i = 0; i < str.size(); ) {
        unsigned char c = str[i];
        uint32_t cp = 0;
        if ((c & 0x80) == 0) { cp = c; i += 1; }
        else if ((c & 0xE0) == 0xC0) { cp = ((c & 0x1F) << 6) | (str[i+1] & 0x3F); i += 2; }
        else if ((c & 0xF0) == 0xE0) { cp = ((c & 0x0F) << 12) | ((str[i+1] & 0x3F) << 6) | (str[i+2] & 0x3F); i += 3; }
        else if ((c & 0xF8) == 0xF0) { cp = ((c & 0x07) << 18) | ((str[i+1] & 0x3F) << 12) | ((str[i+2] & 0x3F) << 6) | (str[i+3] & 0x3F); i += 4; }
        else { i += 1; }
        result.push_back(cp);
    }
    return result;
}

static std::string codepointToUtf8(uint32_t cp) {
    std::string result;
    if (cp <= 0x7F) { result += (char)cp; }
    else if (cp <= 0x7FF) { result += (char)(0xC0 | ((cp >> 6) & 0x1F)); result += (char)(0x80 | (cp & 0x3F)); }
    else if (cp <= 0xFFFF) { result += (char)(0xE0 | ((cp >> 12) & 0x0F)); result += (char)(0x80 | ((cp >> 6) & 0x3F)); result += (char)(0x80 | (cp & 0x3F)); }
    else { result += (char)(0xF0 | ((cp >> 18) & 0x07)); result += (char)(0x80 | ((cp >> 12) & 0x3F)); result += (char)(0x80 | ((cp >> 6) & 0x3F)); result += (char)(0x80 | (cp & 0x3F)); }
    return result;
}

static std::string regionalToCode(uint32_t c1, uint32_t c2) {
    const uint32_t BASE = 0x1F1E6;
    const char A = 'A';
    if (c1 >= BASE && c1 <= BASE + 25 && c2 >= BASE && c2 <= BASE + 25)
        return std::string() + char((c1 - BASE) + A) + char((c2 - BASE) + A);
    return "";
}

static std::string codeToRegional(const std::string& code) {
    if (code.length() != 2) return "";
    const uint32_t BASE = 0x1F1E6;
    const char A = 'A';
    return codepointToUtf8(BASE + (code[0] - A)) + codepointToUtf8(BASE + (code[1] - A));
}

// ======================== dataset ========================

static const std::unordered_map<std::string, std::string> codeToName = {
    // Europe
    {"AD", "Andorra"},
    {"AL", "Albania"},
    {"AT", "Austria"},
    {"BA", "Bosnia and Herzegovina"},
    {"BE", "Belgium"},
    {"BG", "Bulgaria"},
    {"BY", "Belarus"},
    {"CH", "Switzerland"},
    {"CY", "Cyprus"},
    {"CZ", "Czech Republic"},
    {"DE", "Germany"},
    {"DK", "Denmark"},
    {"EE", "Estonia"},
    {"ES", "Spain"},
    {"FI", "Finland"},
    {"FR", "France"},
    {"GB", "United Kingdom"},
    {"GR", "Greece"},
    {"HR", "Croatia"},
    {"HU", "Hungary"},
    {"IE", "Ireland"},
    {"IS", "Iceland"},
    {"IT", "Italy"},
    {"LI", "Liechtenstein"},
    {"LT", "Lithuania"},
    {"LU", "Luxembourg"},
    {"LV", "Latvia"},
    {"MC", "Monaco"},
    {"MD", "Moldova"},
    {"ME", "Montenegro"},
    {"MK", "North Macedonia"},
    {"MT", "Malta"},
    {"NL", "Netherlands"},
    {"NO", "Norway"},
    {"PL", "Poland"},
    {"PT", "Portugal"},
    {"RO", "Romania"},
    {"RS", "Serbia"},
    {"RU", "Russia"},
    {"SE", "Sweden"},
    {"SI", "Slovenia"},
    {"SK", "Slovakia"},
    {"SM", "San Marino"},
    {"TR", "Turkey"},
    {"UA", "Ukraine"},
    {"VA", "Vatican City"},
    {"XK", "Kosovo"},
    // North America
    {"AG", "Antigua and Barbuda"},
    {"AI", "Anguilla"},
    {"AW", "Aruba"},
    {"BB", "Barbados"},
    {"BL", "Saint Barthelemy"},
    {"BM", "Bermuda"},
    {"BS", "Bahamas"},
    {"BZ", "Belize"},
    {"CA", "Canada"},
    {"CR", "Costa Rica"},
    {"CU", "Cuba"},
    {"CW", "Curacao"},
    {"DM", "Dominica"},
    {"DO", "Dominican Republic"},
    {"GD", "Grenada"},
    {"GL", "Greenland"},
    {"GP", "Guadeloupe"},
    {"GT", "Guatemala"},
    {"HN", "Honduras"},
    {"HT", "Haiti"},
    {"JM", "Jamaica"},
    {"KN", "Saint Kitts and Nevis"},
    {"KY", "Cayman Islands"},
    {"LC", "Saint Lucia"},
    {"MF", "Saint Martin"},
    {"MQ", "Martinique"},
    {"MS", "Montserrat"},
    {"MX", "Mexico"},
    {"NI", "Nicaragua"},
    {"PA", "Panama"},
    {"PM", "Saint Pierre and Miquelon"},
    {"PR", "Puerto Rico"},
    {"SV", "El Salvador"},
    {"TC", "Turks and Caicos Islands"},
    {"TT", "Trinidad and Tobago"},
    {"US", "United States"},
    {"VC", "Saint Vincent and the Grenadines"},
    {"VG", "British Virgin Islands"},
    {"VI", "US Virgin Islands"},
    // South America
    {"AR", "Argentina"},
    {"BO", "Bolivia"},
    {"BR", "Brazil"},
    {"CL", "Chile"},
    {"CO", "Colombia"},
    {"EC", "Ecuador"},
    {"FK", "Falkland Islands"},
    {"GF", "French Guiana"},
    {"GY", "Guyana"},
    {"PE", "Peru"},
    {"PY", "Paraguay"},
    {"SR", "Suriname"},
    {"UY", "Uruguay"},
    {"VE", "Venezuela"},
    // Africa
    {"AO", "Angola"},
    {"BF", "Burkina Faso"},
    {"BI", "Burundi"},
    {"BJ", "Benin"},
    {"BW", "Botswana"},
    {"CD", "Democratic Republic of the Congo"},
    {"CF", "Central African Republic"},
    {"CG", "Republic of the Congo"},
    {"CI", "Ivory Coast"},
    {"CM", "Cameroon"},
    {"CV", "Cape Verde"},
    {"DJ", "Djibouti"},
    {"DZ", "Algeria"},
    {"EG", "Egypt"},
    {"EH", "Western Sahara"},
    {"ER", "Eritrea"},
    {"ET", "Ethiopia"},
    {"GA", "Gabon"},
    {"GH", "Ghana"},
    {"GM", "Gambia"},
    {"GN", "Guinea"},
    {"GQ", "Equatorial Guinea"},
    {"GW", "Guinea-Bissau"},
    {"KE", "Kenya"},
    {"KM", "Comoros"},
    {"LR", "Liberia"},
    {"LS", "Lesotho"},
    {"LY", "Libya"},
    {"MA", "Morocco"},
    {"MG", "Madagascar"},
    {"ML", "Mali"},
    {"MR", "Mauritania"},
    {"MU", "Mauritius"},
    {"MW", "Malawi"},
    {"MZ", "Mozambique"},
    {"NA", "Namibia"},
    {"NE", "Niger"},
    {"NG", "Nigeria"},
    {"RE", "Reunion"},
    {"RW", "Rwanda"},
    {"SC", "Seychelles"},
    {"SD", "Sudan"},
    {"SH", "Saint Helena"},
    {"SL", "Sierra Leone"},
    {"SN", "Senegal"},
    {"SO", "Somalia"},
    {"SS", "South Sudan"},
    {"ST", "Sao Tome and Principe"},
    {"SZ", "Eswatini"},
    {"TD", "Chad"},
    {"TG", "Togo"},
    {"TN", "Tunisia"},
    {"TZ", "Tanzania"},
    {"UG", "Uganda"},
    {"YT", "Mayotte"},
    {"ZA", "South Africa"},
    {"ZM", "Zambia"},
    {"ZW", "Zimbabwe"},
    // Asia
    {"AE", "United Arab Emirates"},
    {"AF", "Afghanistan"},
    {"AM", "Armenia"},
    {"AZ", "Azerbaijan"},
    {"BD", "Bangladesh"},
    {"BH", "Bahrain"},
    {"BN", "Brunei"},
    {"BT", "Bhutan"},
    {"CN", "China"},
    {"GE", "Georgia"},
    {"HK", "Hong Kong"},
    {"ID", "Indonesia"},
    {"IL", "Israel"},
    {"IN", "India"},
    {"IQ", "Iraq"},
    {"IR", "Iran"},
    {"JO", "Jordan"},
    {"JP", "Japan"},
    {"KG", "Kyrgyzstan"},
    {"KH", "Cambodia"},
    {"KP", "North Korea"},
    {"KR", "South Korea"},
    {"KW", "Kuwait"},
    {"KZ", "Kazakhstan"},
    {"LA", "Laos"},
    {"LB", "Lebanon"},
    {"LK", "Sri Lanka"},
    {"MM", "Myanmar"},
    {"MN", "Mongolia"},
    {"MO", "Macau"},
    {"MV", "Maldives"},
    {"MY", "Malaysia"},
    {"NP", "Nepal"},
    {"OM", "Oman"},
    {"PH", "Philippines"},
    {"PK", "Pakistan"},
    {"PS", "Palestine"},
    {"QA", "Qatar"},
    {"SA", "Saudi Arabia"},
    {"SG", "Singapore"},
    {"SY", "Syria"},
    {"TH", "Thailand"},
    {"TJ", "Tajikistan"},
    {"TL", "East Timor"},
    {"TM", "Turkmenistan"},
    {"TW", "Taiwan"},
    {"UZ", "Uzbekistan"},
    {"VN", "Vietnam"},
    {"YE", "Yemen"},
    // Oceania
    {"AS", "American Samoa"},
    {"AU", "Australia"},
    {"CC", "Cocos Islands"},
    {"CK", "Cook Islands"},
    {"CX", "Christmas Island"},
    {"FJ", "Fiji"},
    {"FM", "Micronesia"},
    {"GU", "Guam"},
    {"KI", "Kiribati"},
    {"MH", "Marshall Islands"},
    {"MP", "Northern Mariana Islands"},
    {"NC", "New Caledonia"},
    {"NF", "Norfolk Island"},
    {"NR", "Nauru"},
    {"NU", "Niue"},
    {"NZ", "New Zealand"},
    {"PF", "French Polynesia"},
    {"PG", "Papua New Guinea"},
    {"PW", "Palau"},
    {"SB", "Solomon Islands"},
    {"TK", "Tokelau"},
    {"TO", "Tonga"},
    {"TV", "Tuvalu"},
    {"VU", "Vanuatu"},
    {"WF", "Wallis and Futuna"},
    {"WS", "Samoa"},
    // Other
    {"AQ", "Antarctica"},
    {"BV", "Bouvet Island"},
    {"GS", "South Georgia and the South Sandwich Islands"},
    {"HM", "Heard Island and McDonald Islands"},
    {"IO", "British Indian Ocean Territory"},
    {"TF", "French Southern Territories"},
    {"UM", "US Minor Outlying Islands"},
};

static std::unordered_map<std::string, std::string> nameToCode() {
    std::unordered_map<std::string, std::string> result;
    for (const auto& pair : codeToName) {
        result[pair.second] = pair.first;
        std::string lower = pair.second;
        for (char& c : lower) c = tolower(c);
        result[lower] = pair.first;
    }
    return result;
}

// ======================== internal ========================

// Walk through str and replace every flag using a converter function
static std::string _replaceFlags(const std::string& str,
                                  const std::function<std::string(uint32_t, uint32_t)>& converter) {
    auto cp = utf8ToCodepoints(str);
    std::string result;
    const uint32_t BASE = 0x1F1E6;
    for (size_t i = 0; i < cp.size(); i++) {
        if (i + 1 < cp.size() &&
            cp[i] >= BASE && cp[i] <= BASE + 25 &&
            cp[i+1] >= BASE && cp[i+1] <= BASE + 25) {
            result += converter(cp[i], cp[i+1]);
            i++;
        } else {
            result += codepointToUtf8(cp[i]);
        }
    }
    return result;
}

// ======================== funcs ========================

// Is emoji flag in str
inline bool isFlagIn(const std::string& str) {
    auto cp = utf8ToCodepoints(str);
    const uint32_t BASE = 0x1F1E6;
    for (size_t i = 0; i + 1 < cp.size(); i++) {
        if (cp[i] >= BASE && cp[i] <= BASE + 25 &&
            cp[i+1] >= BASE && cp[i+1] <= BASE + 25)
            return true;
    }
    return false;
}

// Flag to Letters 🇪🇪 → "EE"
inline std::string FtoL(const std::string& flag) {
    auto cp = utf8ToCodepoints(flag);
    if (cp.size() < 2) return "";
    return regionalToCode(cp[0], cp[1]);
}

// Letters to Flag "EE" → 🇪🇪
inline std::string LtoF(const std::string& code) {
    return codeToRegional(code);
}

// Flag to Word 🇳🇱 → "Netherlands"
inline std::string FtoW(const std::string& flag) {
    std::string code = FtoL(flag);
    if (code.empty()) return "";
    auto it = codeToName.find(code);
    return (it != codeToName.end()) ? it->second : "";
}

// Flag to Word lowercase 🇳🇱 → "netherlands"
inline std::string FtoW_l(const std::string& flag) {
    std::string result = FtoW(flag);
    for (char& c : result) c = tolower(c);
    return result;
}

// Word to Flag "Netherlands" → 🇳🇱
inline std::string WtoF(const std::string& country_name) {
    static auto nameToCodeMap = nameToCode();
    auto it = nameToCodeMap.find(country_name);
    return (it != nameToCodeMap.end()) ? LtoF(it->second) : "";
}

// Word to Letters "Netherlands" → "NL"
inline std::string WtoL(const std::string& country_name) {
    static auto nameToCodeMap = nameToCode();
    auto it = nameToCodeMap.find(country_name);
    return (it != nameToCodeMap.end()) ? it->second : "";
}

// Letters to Word "NL" → "Netherlands"
inline std::string LtoW(const std::string& code) {
    auto it = codeToName.find(code);
    return (it != codeToName.end()) ? it->second : "";
}

// Letters to Word lowercase "NL" → "netherlands"
inline std::string LtoW_l(const std::string& code) {
    std::string result = LtoW(code);
    for (char& c : result) c = tolower(c);
    return result;
}

// Extract codes from all flags in str
inline std::vector<std::string> extractCodes(const std::string& str) {
    std::vector<std::string> result;
    auto cp = utf8ToCodepoints(str);
    const uint32_t BASE = 0x1F1E6;
    for (size_t i = 0; i + 1 < cp.size(); i++) {
        if (cp[i] >= BASE && cp[i] <= BASE + 25 &&
            cp[i+1] >= BASE && cp[i+1] <= BASE + 25) {
            std::string code = regionalToCode(cp[i], cp[i+1]);
            if (!code.empty()) result.push_back(code);
            i++;
        }
    }
    return result;
}

// Extract code from first flag in str
inline std::string extractCode(const std::string& str) {
    auto codes = extractCodes(str);
    return codes.empty() ? "" : codes[0];
}

// Replace all flags in str with their codes: "🇳🇱-server" → "NL-server"
inline std::string replaceFlagsToCode(const std::string& str) {
    return _replaceFlags(str, [](uint32_t c1, uint32_t c2) {
        return regionalToCode(c1, c2);
    });
}

// Replace all flags in str with country names: "🇳🇱-server" → "Netherlands-server"
inline std::string replaceFlagsToName(const std::string& str) {
    return _replaceFlags(str, [](uint32_t c1, uint32_t c2) {
        std::string code = regionalToCode(c1, c2);
        auto it = codeToName.find(code);
        return (it != codeToName.end()) ? it->second : code;
    });
}


// Replace all flags in str with country names lowercase: "🇳🇱-server" → "netherlands-server"
inline std::string replaceFlagsToName_l(const std::string& str) {
    return _replaceFlags(str, [](uint32_t c1, uint32_t c2) {
        std::string code = regionalToCode(c1, c2);
        auto it = codeToName.find(code);
        std::string name = (it != codeToName.end()) ? it->second : code;
        for (char& c : name) c = tolower(c);
        return name;
    });
}

// Replace all codes in str with country names: "NL-server" → "Netherlands-server"
inline std::string replaceCodeToWord(const std::string& str) {
    std::string result = str;
    for (const auto& pair : codeToName) {
        size_t pos = 0;
        while ((pos = result.find(pair.first, pos)) != std::string::npos) {
            // проверяем что это отдельное слово, а не часть другого
            bool leftOk  = (pos == 0 || !isupper(result[pos - 1]));
            bool rightOk = (pos + 2 >= result.size() || !isupper(result[pos + 2]));
            if (leftOk && rightOk) {
                result.replace(pos, 2, pair.second);
                pos += pair.second.size();
            } else {
                pos += 2;
            }
        }
    }
    return result;
}

// Replace all codes in str with flags: "NL-server" → "🇳🇱-server"
inline std::string replaceCodeToFlag(const std::string& str) {
    std::string result = str;
    for (const auto& pair : codeToName) {
        size_t pos = 0;
        while ((pos = result.find(pair.first, pos)) != std::string::npos) {
            bool leftOk  = (pos == 0 || !isupper(result[pos - 1]));
            bool rightOk = (pos + 2 >= result.size() || !isupper(result[pos + 2]));
            if (leftOk && rightOk) {
                std::string flag = codeToRegional(pair.first);
                result.replace(pos, 2, flag);
                pos += flag.size();
            } else {
                pos += 2;
            }
        }
    }
    return result;
}

// Get all known codes
inline std::vector<std::string> allCodes() {
    std::vector<std::string> result;
    result.reserve(codeToName.size());
    for (const auto& pair : codeToName)
        result.push_back(pair.first);
    return result;
}

// Get all country names
inline std::vector<std::string> allNames() {
    std::vector<std::string> result;
    result.reserve(codeToName.size());
    for (const auto& pair : codeToName)
        result.push_back(pair.second);
    return result;
}

// Get all country names lowercase
inline std::vector<std::string> allNames_l() {
    std::vector<std::string> result;
    result.reserve(codeToName.size());
    for (const auto& pair : codeToName) {
        std::string lower = pair.second;
        for (char& c : lower) c = tolower(c);
        result.push_back(lower);
    }
    return result;
}

// Get all flags as emoji strings
inline std::vector<std::string> allFlags() {
    std::vector<std::string> result;
    result.reserve(codeToName.size());
    for (const auto& pair : codeToName)
        result.push_back(codeToRegional(pair.first));
    return result;
}

// Check if input is a known country — accepts flag, code, or name (any case)
inline bool isCountryIn(const std::string& input) {
    if (input.empty()) return false;

    if (isFlagIn(input)) {
        std::string code = extractCode(input);
        return codeToName.find(code) != codeToName.end();
    }

    if (input.length() == 2 && isupper(input[0]) && isupper(input[1]))
        return codeToName.find(input) != codeToName.end();

    static auto nameToCodeMap = nameToCode();
    std::string lower = input;
    for (char& c : lower) c = tolower(c);
    return nameToCodeMap.find(lower) != nameToCodeMap.end();
}

} // namespace scftl

#endif // SCFTL_HPP