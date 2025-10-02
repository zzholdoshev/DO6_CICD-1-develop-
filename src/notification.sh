#!bin/bash

curl -X POST -H "Content-Type: application/json" -d "{\"chat_id\": \"-4150393227\", \"text\": \"Project: $CI_PROJECT_NAME\nStage: $CI_JOB_STAGE\nStatus: $CI_JOB_STATUS\"}" https://api.telegram.org/bot7012324936:AAEFj0rGMkEwc-Q7IxBYZJONkABWUsWQDLg/sendMessage 
