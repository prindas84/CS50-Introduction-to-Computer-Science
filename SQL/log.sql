-- Query the crime scene report for more information.

SELECT description FROM crime_scene_reports

WHERE street LIKE "%Chamberlin%" AND day = "28" AND month = "7" AND year = "2020";

        -- NOTES: The crime took place at 10:15am at the Chamberlin Street courthouse. There were 3 witnesses, all of whom mentioned the courthouse.

-- Query the interviews to find who was interviewed and who said what.

SELECT name, transcript FROM interviews

WHERE day = "28" AND month = "7" AND year = "2020" AND transcript LIKE "%courthouse%";

        -- NOTES: The people who were interviewed were Ruth, Eugene and Raymond.
            -- The theif drove from the courthouse parking between 10:15am and 10:25am.
            -- The theif withdrew money from an ATM on Fifer Street before 10:15am.
            -- The theif made a phone call after 10:15am that was less than 60 seconds long.
            -- The theif planned on buying the earliest plane ticket out of Fiftyville on the 29th July.
            -- The ticket was purchased for the theif by the accomplice.
                -- TO DO: Check the courthouse parking camera footage.
                -- TO DO: Check the ATM footage.
                -- TO DO: Check the length of calls made after 10:15am that were less than 60 seconds.
                -- TO DO: Check flights out of Fiftyville on the 29th July.
                -- TO DO: Check who purchased the flight out of Fiftyville.

-- Query the courthouse secuirty logs to see who left the parking lot between 10:15am and 10:25am.

SELECT * FROM courthouse_security_logs

WHERE day = "28" AND month = "7" AND year = "2020" AND hour = 10 AND minute >= 15 AND minute <= 25 AND activity = "exit";

-- Query the people table to see who are the owners of the cars listed above.

SELECT name FROM people

WHERE license_plate IN
    (SELECT license_plate FROM courthouse_security_logs
        WHERE day = "28" AND month = "7" AND year = "2020" AND hour = 10 AND minute >= 15 AND minute <= 25 AND activity = "exit");

        -- POTENTIAL SUSPECTS: Patrick, Amber, Elizabeth, Roger, Danielle, Russell, Evelyn, Ernest

-- Query the ATM transactions to see who withdrew money from an ATM on Fifer Street before 10:15am.

SELECT * FROM atm_transactions

WHERE atm_location LIKE "%Fifer%" AND day = "28" AND month = "7" AND year = "2020" AND transaction_type = "withdraw";

-- Query the people table and the bank accounts table to see which people made the withdrawls listed above.

SELECT name FROM people

WHERE id IN
    (SELECT person_id FROM bank_accounts WHERE account_number IN
        (SELECT account_number FROM atm_transactions WHERE atm_location LIKE "%Fifer%" AND day = "28" AND month = "7" AND year = "2020" AND transaction_type = "withdraw"));

        -- POTENTIAL SUSPECTS: Bobby, Elizabeth, Victoria, Madison, Roy, Danielle, Russell, Ernest

-- Compare both suspect lists created.

SELECT name FROM people

WHERE license_plate IN
    (SELECT license_plate FROM courthouse_security_logs
        WHERE day = "28" AND month = "7" AND year = "2020" AND hour = 10 AND minute >= 15 AND minute <= 25 AND activity = "exit")

AND id IN
    (SELECT person_id FROM bank_accounts WHERE account_number IN
        (SELECT account_number FROM atm_transactions WHERE atm_location LIKE "%Fifer%" AND day = "28" AND month = "7" AND year = "2020" AND transaction_type = "withdraw"));

        -- POTENTIAL SUSPECTS: Elizabeth, Danielle, Russell, Ernest

-- Query the phone calls table to find out who made a phone call after 10:15am that was less than 60 seconds long.

SELECT * FROM phone_calls

WHERE day = "28" AND month = "7" AND year = "2020" AND duration <= 60;

-- Query the people table to find out who made the calls listed above.

SELECT name FROM people

WHERE phone_number IN
    (SELECT caller FROM phone_calls WHERE day = "28" AND month = "7" AND year = "2020" AND duration <= 60);

-- Compare the latest suspect list to the other two suspect lists.

SELECT name FROM people

WHERE license_plate IN
    (SELECT license_plate FROM courthouse_security_logs
        WHERE day = "28" AND month = "7" AND year = "2020" AND hour = 10 AND minute >= 15 AND minute <= 25 AND activity = "exit")

AND id IN
    (SELECT person_id FROM bank_accounts WHERE account_number IN
        (SELECT account_number FROM atm_transactions WHERE atm_location LIKE "%Fifer%" AND day = "28" AND month = "7" AND year = "2020" AND transaction_type = "withdraw"))

AND phone_number IN
    (SELECT caller FROM phone_calls WHERE day = "28" AND month = "7" AND year = "2020" AND duration <= 60);

        -- POTENTIAL SUSPECTS: Russell, Ernest

-- Query the flights table to find the earliest flight out of Fiftyville on the 29th July.

SELECT * FROM flights

WHERE day = "29" AND month = "7" AND year = "2020" AND origin_airport_id IN
    (SELECT id FROM airports WHERE city LIKE "Fiftyville");

        -- NOTES: The earliest flight out of Fiftyville is flight 36 at 8:20am.

-- Query the flights table to find the destination of the 8:20am flight.

SELECT city FROM airports

WHERE id IN
    (SELECT destination_airport_id FROM flights WHERE day = "29" AND month = "7" AND year = "2020" AND hour = 8 and minute = 20 AND origin_airport_id IN
        (SELECT id FROM airports WHERE city LIKE "Fiftyville"));

        -- The flight was going to London.
        -- THE THEIF ESCAPED TO LONDON.

-- Query the people database to find who was on flight 36 to London at 8:20am.

SELECT name FROM people

WHERE passport_number IN
    (SELECT passport_number FROM passengers WHERE flight_id IN
        (SELECT id FROM flights WHERE day = "29" AND month = "7" AND year = "2020" AND hour = 8 and minute = 20 AND origin_airport_id IN
            (SELECT id FROM airports WHERE city LIKE "Fiftyville")));

-- Compare the latest suspect list to the other three suspect lists.

SELECT name FROM people

WHERE license_plate IN
    (SELECT license_plate FROM courthouse_security_logs
        WHERE day = "28" AND month = "7" AND year = "2020" AND hour = 10 AND minute >= 15 AND minute <= 25 AND activity = "exit")

AND id IN
    (SELECT person_id FROM bank_accounts WHERE account_number IN
        (SELECT account_number FROM atm_transactions WHERE atm_location LIKE "%Fifer%" AND day = "28" AND month = "7" AND year = "2020" AND transaction_type = "withdraw"))

AND phone_number IN
    (SELECT caller FROM phone_calls WHERE day = "28" AND month = "7" AND year = "2020" AND duration <= 60)

AND passport_number IN
    (SELECT passport_number FROM passengers WHERE flight_id IN
        (SELECT id FROM flights WHERE day = "29" AND month = "7" AND year = "2020" AND hour = 8 and minute = 20 AND origin_airport_id IN
            (SELECT id FROM airports WHERE city LIKE "Fiftyville")));

        -- THE THEIF IS ERNEST!!!

-- Query the phone calls table to find who was the accomplice. The accomplice is the person who Ernest called after the robbery.

SELECT name FROM people

WHERE phone_number IN
    (SELECT receiver FROM phone_calls WHERE day = "28" AND month = "7" AND year = "2020" AND duration <= 60 AND caller IN
        (SELECT phone_number FROM people WHERE name = "Ernest"));

        -- THE ACCOMPLICE IS BERTHOLD!!!