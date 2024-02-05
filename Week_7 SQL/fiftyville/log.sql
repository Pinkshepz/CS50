-- Keep a log of any SQL queries you execute as you solve the mystery.

-- 1. Query crime record on 28 July 2021 at Humphrey Street>
SELECT id, description 
FROM crime_scene_reports
WHERE day = 28
AND month = 7
AND year = 2021
AND street = "Humphrey Street";

-- OUTPUT Description
-- 295 | Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery. Interviews were conducted today with three witnesses who were present at the time – each of their interview transcripts mentions the bakery.
-- 297 | Littering took place at 16:36. No known witnesses.

-- 2. We found that crime id is 295 where the duck is stolen. Search for interviews on that day>
SELECT name, transcript 
FROM interviews
WHERE day = 28
AND month = 7
AND year = 2021;

-- OUTPUT Three corresponding interview transcripts
-- Ruth | Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away. If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame.
-- Eugene | I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at Emma's bakery, I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.
-- Raymond | As the thief was leaving the bakery, they called someone who talked to them for less than a minute. In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow. The thief then asked the person on the other end of the phone to purchase the flight ticket.

-- 3. Check security camera > select name of suspects who enter and exit store
-- 4. Check ATM transactions
-- 5. Check phone call records > caller
SELECT name
FROM Bakery_security_logs T1, Bakery_security_logs T2, people
WHERE T1.day = 28 AND T1.month = 7 AND T1.year = 2021
AND T2.day = 28 AND T2.month = 7 AND T2.year = 2021
AND T1.license_plate = T2.license_plate
AND T1.license_plate = people.license_plate
AND T1.activity = "entrance"
AND T2.activity = "exit"
INTERSECT
SELECT name
FROM atm_transactions, bank_accounts, people
WHERE day = 28 AND month = 7 AND year = 2021
AND atm_location = "Leggett Street"
AND transaction_type = "withdraw"
AND atm_transactions.account_number = bank_accounts.account_number
AND bank_accounts.person_id = people.id
INTERSECT
SELECT name FROM phone_calls, people
WHERE day = 28 AND month = 7 AND year = 2021
AND duration < 60
AND phone_calls.caller = people.phone_number;

-- 6. Check flight records
SELECT name FROM flights, airports, passengers, people
WHERE day = 29 AND month = 7 AND year = 2021
AND city = "Fiftyville"
AND flights.origin_airport_id = airports.id
AND flights.id = passengers.flight_id
AND passengers.passport_number = people.passport_number
ORDER BY flights.hour
LIMIT 10;

-- We found 3 suspects: Bruce Diana Taylor
-- We found Bruce took the earliest flight among suspects
-- So, we hypothesize that Bruce is the theif

-- 7. Check Bruce's flight info
SELECT * FROM flights, airports, passengers, people
WHERE day = 29 AND month = 7 AND year = 2021
AND city = "Fiftyville"
AND name = "Bruce"
AND flights.origin_airport_id = airports.id
AND flights.id = passengers.flight_id
AND passengers.passport_number = people.passport_number
ORDER BY flights.hour
LIMIT 10;

-- OUTPUT > 36|8|4|2021|7|29|8|20 > flight time: 8.20 am, flight destination id: 4
-- 8. Find flight destination
SELECT city FROM airports
WHERE id = 4;

-- The destination is New York City

-- 9. Check theif personal data
SELECT * FROM people
WHERE name = "Bruce";

-- Bruce phone number is (367) 555-5533

-- 10. Check phone calls records to find potential accomplice
SELECT name FROM people
WHERE phone_number =
(
    SELECT receiver FROM phone_calls
    WHERE day = 28 AND month = 7 AND year = 2021
    AND duration < 60
    AND phone_calls.caller = "(367) 555-5533"
);

-- The potential accomplice is Robin