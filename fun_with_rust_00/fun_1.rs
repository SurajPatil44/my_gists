#![allow(dead_code)]
#![allow(unused_variables)]
use std::str::FromStr;

#[derive(Debug)]
enum ParseError {
    DayParseError(String),
    MonthParseError(String),
} 

#[derive(Debug, Eq, PartialEq, Ord, PartialOrd)]
struct Day(u8);

impl Day {
    fn min_value() -> Self {
        Day(1)
    }
    fn max_value() -> Self {
        Day(31)
    }
}

#[derive(Debug, Eq, PartialEq, Ord, PartialOrd)]
struct Month(u8);

impl Month {
    fn min_value() -> Self {
        Month(1)
    }
    fn max_value() -> Self {
        Month(12)
    }
}

#[derive(Debug)]
struct Date {
    day: Day,
    month: Month,
    year: Option<u16>,
}

struct DateBuilder;

impl DateBuilder {
    fn from_args(day: Day, month: Month, year: Option<u16>) -> Result<Date, ParseError> {
        if day < Day::min_value() || day > Day::max_value() {
            return Err(ParseError::DayParseError(day.0.to_string()));
        }
        if month < Month::min_value() || month > Month::max_value() {
            return Err(ParseError::MonthParseError(month.0.to_string()));
        }
        Ok(Date { day, month, year })
    }
}

impl Date {
    fn from_string(date_string: &str, date_format: &str) -> Result<Date, ParseError> {
        //let mut _day = Ok(0);
        let date_index = date_format.find("dd");
        let mon_index = date_format.find("mm");
        let day = match date_index {
            Some(idx) => match u8::from_str(&date_string[idx..idx + 2]) {
                Ok(d) => Some(d),
                Err(_) => return Err(ParseError::DayParseError(String::from(&date_string[idx..idx + 2]))),
            },
            None => return Err(ParseError::DayParseError(String::from("no dd in format"))),
        };
        let month = match mon_index {
            Some(idx) => match u8::from_str(&date_string[idx..idx + 2]) {
                Ok(d) => Some(d),
                Err(_) => return Err(ParseError::MonthParseError(String::from(&date_string[idx..idx + 2]))),
            },
            None => return Err(ParseError::MonthParseError(String::from("no mm in format"))),
        };
        let yr_index = date_format.find("yyyy");
        let year = match yr_index {
            Some(idx) => {
                // Bound checking
                if (idx + 4) <= date_string.len() {
                    match u16::from_str(&date_string[idx..idx + 4]) {
                        Ok(d) => Some(d),
                        Err(_) => None,
                    }
                } else {
                    None
                }
            }
            None => None,
        };
        let day = day.unwrap();
        let month = month.unwrap();
        DateBuilder::from_args(Day(day), Month(month), year)
    }
}

fn main() {
    let date_string = "gg/12/1994";
    let date_format = "dd/mm/yyyy";

    let date = Date::from_string(date_string, date_format);
    println!("{:?}", date);
}
